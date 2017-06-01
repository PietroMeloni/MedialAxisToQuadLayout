/*
 * @author    Alessandro Muntoni (muntoni.alessandro@gmail.com)
 * @copyright Alessandro Muntoni 2016.
 */

#include <boost/tokenizer.hpp>
#include "dcel_face_iterators.h"
#include "dcel_vertex_iterators.h"
#include "dcel_iterators.h"
#include <common/comparators.h>
#include <common/utils.h>
#include <common/load_save_file.h>

#ifdef CGAL_DEFINED
#include <cgal/cgalinterface.h>
#endif

#ifdef EIGENMESH_DEFINED
#include <eigenmesh/eigenmesh.h>
#endif

/****************
 * Constructors *
 ****************/

/**
 * \~Italian
 * @brief Costruttore vuoto.
 *
 * Crea una Dcel vuota, con 0 vertici, 0 half edge e 0 facce.
 */
Dcel::Dcel() : nVertices(0), nHalfEdges(0), nFaces(0) {
}

/**
 * \~Italian
 * @brief Costruttore di copia di una Dcel.
 *
 * Crea una nuova Dcel a partire dalla Dcel passata in input, creando nuove istanze
 * di tutti i vertici, gli half edge e le facce contenute in dcel, e settando di conseguenza
 * tutte le relazioni tra essi.
 * @param[in] dcel: dcel da cui verrà creata la Dcel this.
 */
Dcel::Dcel(const Dcel& dcel) {
    copyFrom(dcel);
}

/*Dcel::Dcel(Dcel&& dcel) {
    vertices = std::move(dcel.vertices);
    faces = std::move(dcel.faces);
    halfEdges = std::move(dcel.halfEdges);
    unusedVids = std::move(dcel.unusedVids);
    unusedHeids = std::move(dcel.unusedHeids);
    unusedFids = std::move(dcel.unusedFids);
    nVertices = std::move(dcel.nVertices);
    nHalfEdges = std::move(dcel.nHalfEdges);
    nFaces = std::move(dcel.nFaces);
    boundingBox = std::move(dcel.boundingBox);
}*/

#ifdef EIGENMESH_DEFINED
Dcel::Dcel(const SimpleEigenMesh& eigenMesh) {
    copyFrom(eigenMesh);
}

Dcel::Dcel(const EigenMesh& eigenMesh) {
    copyFrom(eigenMesh);
}
#endif


/**
 * \~Italian
 * @brief Distruttore della Dcel.
 *
 * Elimina tutti gli elementi contenuti nelle liste dei vertici, degli half edge e delle facce della Dcel.
 */
Dcel::~Dcel() {
    for (unsigned int i=0; i<vertices.size(); i++)
        if (vertices[i]!= nullptr)
            delete vertices[i];
    for (unsigned int i=0; i<halfEdges.size(); i++)
        if (halfEdges[i] != nullptr)
            delete halfEdges[i];
    for (unsigned int i=0; i<faces.size(); i++)
        if (faces[i] != nullptr)
            delete faces[i];
}

/******************
* Public Methods *
******************/

/**
 * \~Italian
 * @brief Funzione che verifica se la mesh contenuta nella Dcel è una mesh di triangoli
 * @warning utilizza Dcel::Face::constIncidentVertexIterator
 * @return True se la mesh è composta da soli triangoli, false altrimenti
 * @par Complessità:
 *      \e O(numFaces)
 */
bool Dcel::isTriangleMesh() const {
    if (getNumberFaces() == 0) return false;
    ConstFaceIterator fit;
    for (fit = faceBegin(); fit != faceEnd(); ++fit)
        if (! (*fit)->isTriangle()) return false;
    return true;
}

/**
 * \~Italian
 * @brief Funzione che restituisce la somma delle aree di tutta le facce presenti nella Dcel.
 * @return L'area dell'intra superficie della mesh
 * @par Complessità:
 *      \e O(numFaces)
 */
double Dcel::getSurfaceArea() const {
    double area = 0;
    for (ConstFaceIterator fit = faceBegin(); fit != faceEnd(); ++fit){
        area += (*fit)->getArea();
    }
    return area;
}

Pointd Dcel::getBarycenter() const {
    Pointd bc;
    for (const Dcel::Vertex* v : vertexIterator()){
        bc += v->getCoordinate();
    }
    bc /= getNumberVertices();
    return bc;
}

double Dcel::getAverageHalfEdgesLength() const {
    double average = 0;
    for (const HalfEdge* he : halfEdgeIterator()){
        average += he->getLength();
    }
    average /= nHalfEdges;
    return average;
}

/**
 * \~Italian
 * @brief Salva un file nel formato obj rappresentante la mesh contenuta nella Dcel.
 *
 * @todo Il formato OBJ non gestisce buchi all'interno delle facce, per cui, ogni volta che ci sono
 * buchi, viene creato un singolo bordo che unsice il bordo interno a tutti i bordi interni utilizzando
 * dummy edges. \n
 * Per la gestione dei colori, viene creato un file separato con lo stesso nome e con estensione mtl.
 *
 * @warning Utilizza Dcel::Face::ConstIncidentHalfEdgeIterator
 *
 * @param[in] fileNameObj: il nome del file su cui verrà salvata la mesh, \b con \b l'estensione \b obj.
 *
 * @par Complessità:
 *      \e O(numVertices) + \e O(numFaces) + \e O(numHalfEdges)
 */
void Dcel::saveOnObjFile(std::string fileNameObj) const {
    std::vector<double> vertices;
    std::vector<double> verticesNormals;
    std::vector<int> faces;
    std::vector<unsigned int> faceSizes;
    std::vector<float> faceColors;

    toStdVectors(vertices, verticesNormals, faces, faceSizes, faceColors);

    Common::MeshType meshType = Common::POLYGON_MESH;
    int mode = Common::NORMAL_VERTICES | Common::COLOR_FACES;
    Common::saveMeshOnObj(fileNameObj, getNumberVertices(), getNumberFaces(), vertices.data(), faces.data(), meshType, mode, verticesNormals.data(), Common::RGB, Common::Dummies::dummyVectorFloat.data(), faceColors.data(), faceSizes.data());
}

/**
 * \~Italian
 * @brief Salva un file nel formato ply rappresentante la mesh contenuta nella Dcel.
 *
 * @todo Il formato PLY non gestisce buchi all'interno delle facce, per cui, ogni volta che ci sono
 * buchi, viene creato un singolo bordo che unsice il bordo interno a tutti i bordi interni utilizzando
 * dummy edges. \n
 *
 * @warning Utilizza Dcel::Face::ConstIncidentHalfEdgeIterator
 *
 * @param[in] fileNamePly: il nome del file su cui verrà salvata la mesh, \b con \b l'estensione \b ply.
 *
 * @par Complessità:
 *      \e O(numVertices) + \e O(numFaces) + \e O(numHalfEdges)
 */
void Dcel::saveOnPlyFile(std::string fileNamePly) const {
    std::vector<double> vertices;
    std::vector<double> verticesNormals;
    std::vector<int> faces;
    std::vector<unsigned int> faceSizes;
    std::vector<float> faceColors;

    toStdVectors(vertices, verticesNormals, faces, faceSizes, faceColors);

    Common::MeshType meshType = Common::POLYGON_MESH;
    int mode = Common::NORMAL_VERTICES | Common::COLOR_FACES;
    Common::saveMeshOnPly(fileNamePly, getNumberVertices(), getNumberFaces(), vertices.data(), faces.data(), meshType, mode, verticesNormals.data(), Common::RGB, Common::Dummies::dummyVectorFloat.data(), faceColors.data(), faceSizes.data());
}

/**
 * \~Italian
 * @brief Salva un file nel formato dcel rappresentante la mesh contenuta nella Dcel.
 *
 * Il formato .dcel è un formato creato appositamente per salvataggio e caricamento della classe Dcel.
 *
 * @param[in] fileNameDcel: il nome del file su cui verrà salvata la mesh, \b con \b l'estensione \b dcel.
 *
 * @par Complessità:
 *      \e O(numVertices) + \e O(numFaces) + \e O(numHalfEdges)
 */
void Dcel::saveOnDcelFile(std::string fileNameDcel) const {
    std::ofstream myfile;
    myfile.open (fileNameDcel, std::ios::out | std::ios::binary);
    if (myfile.is_open()) {
        serialize(myfile);
    }
    myfile.close();

}

/**
 * \~Italian
 * @brief Dcel::serialize
 *
 * Serializza la dcel in uno std::ofstream aperto in modalità bin.
 * Lo stream passato in input deve essere già aperto, e a fine funzione punterà al blocco immediatamente successivo
 * all'ultima informazione della dcel salvata.
 * Può essere quindi utilizzata per salvare la dcel in un file binario insieme ad altri dati.
 * Per caricare successivamente la dcel, utilizzare la funzione Dcel::deserialize().
 *
 * @param[in] binaryFile: std::ofstream del file binario dove si vuole salvare il file.
 *
 * @par Complessità:
 *      \e O(numVertices) + \e O(numFaces) + \e O(numHalfEdges)
 */
void Dcel::serialize(std::ofstream &binaryFile) const {
    //BB
    boundingBox.serialize(binaryFile);
    //N
    Serializer::serialize(nVertices, binaryFile);
    Serializer::serialize(nHalfEdges, binaryFile);
    Serializer::serialize(nFaces, binaryFile);
    //Sets
    Serializer::serialize(unusedVids, binaryFile);
    Serializer::serialize(unusedHeids, binaryFile);
    Serializer::serialize(unusedFids, binaryFile);
    //Vertices
    for (ConstVertexIterator vit = vertexBegin(); vit != vertexEnd(); ++vit){
        const Dcel::Vertex* v = *vit;
        int heid = -1;
        if (v->getIncidentHalfEdge() != nullptr) heid = v->getIncidentHalfEdge()->getId();

        Serializer::serialize(v->getId(), binaryFile);
        v->getCoordinate().serialize(binaryFile);
        v->getNormal().serialize(binaryFile);
        Serializer::serialize(heid, binaryFile);
        Serializer::serialize(v->getCardinality(), binaryFile);
        Serializer::serialize(v->getFlag(), binaryFile);
    }
    //HalfEdges
    for (ConstHalfEdgeIterator heit = halfEdgeBegin(); heit != halfEdgeEnd(); ++heit){
        const Dcel::HalfEdge* he = *heit;
        int fv = -1; if (he->getFromVertex() != nullptr) fv = he->getFromVertex()->getId();
        int tv = -1; if (he->getToVertex() != nullptr) tv = he->getToVertex()->getId();
        int tw = -1; if (he->getTwin() != nullptr) tw = he->getTwin()->getId();
        int prev = -1; if (he->getPrev() != nullptr) prev = he->getPrev()->getId();
        int next = -1; if (he->getNext() != nullptr) next = he->getNext()->getId();
        int face = -1; if (he->getFace() != nullptr) face = he->getFace()->getId();

        Serializer::serialize(he->getId(), binaryFile);
        Serializer::serialize(fv, binaryFile);
        Serializer::serialize(tv, binaryFile);
        Serializer::serialize(tw, binaryFile);
        Serializer::serialize(prev, binaryFile);
        Serializer::serialize(next, binaryFile);
        Serializer::serialize(face, binaryFile);
        Serializer::serialize(he->getFlag(), binaryFile);
    }
    //Faces
    for (ConstFaceIterator fit = faceBegin(); fit != faceEnd(); ++fit){
        const Dcel::Face* f = *fit;
        int ohe = -1; if (f->getOuterHalfEdge() != nullptr) ohe = f->getOuterHalfEdge()->getId();
        Serializer::serialize(f->getId(), binaryFile);
        Serializer::serialize(ohe, binaryFile);
        f->getNormal().serialize(binaryFile);
        Serializer::serialize(f->getColor(), binaryFile);
        Serializer::serialize(f->getArea(), binaryFile);
        Serializer::serialize(f->getFlag(), binaryFile);
        Serializer::serialize(f->getNumberInnerHalfEdges(), binaryFile);

        for (Dcel::Face::ConstInnerHalfEdgeIterator heit = f->innerHalfEdgeBegin(); heit != f->innerHalfEdgeEnd(); ++heit){
            const Dcel::HalfEdge* he = *heit;
            int idhe = -1; if (he != nullptr) idhe = he->getId();
            Serializer::serialize(idhe, binaryFile);
        }

    }
}

/**
 * \~Italian
 * @brief Aggiunge un vertice alla Dcel con le coordinate passate in input, e ne restituisce il puntatore
 *
 * Il Dcel::Vertex è inizializzato col costruttore Vertex(Pointd), e successivamente viene settato un id
 * univoco..
 *
 * @param[in] p: le coordinate del vertice da aggiungere alla Dcel
 * @return Il puntatore al vertice appena inserito nella Dcel
 * @par Complessità:
 *      \e O(1)
 */
Dcel::Vertex *Dcel::addVertex(const Pointd& p, const Vec3& n, const Color& c) {
    Vertex* last= new Vertex(*this);
    if (unusedVids.size() == 0) {
        last->setId(nVertices);
        vertices.push_back(last);
        vertexCoordinates.push_back(p);
        vertexNormals.push_back(n);
        vertexColors.push_back(c);
    }
    else {
        int vid = *(unusedVids.begin());
        last->setId(vid);
        vertices[vid] = last;
        vertexCoordinates[vid] = p;
        vertexNormals[vid] = n;
        vertexColors[vid] = c;
        unusedVids.erase(vid);
    }
    nVertices++;
    return last;
}

/**
 * \~Italian
 * @brief Aggiunge un half edge alla Dcel e ne restituisce il puntatore.
 *
 * Il Dcel::HalfEdge è inizializzato col costruttore vuoto, e successivamente viene settato un id
 * univoco.
 *
 * @return Il puntatore all'half edge appena inserito nella Dcel
 * @par Complessità:
 *      \e O(1)
 */
Dcel::HalfEdge* Dcel::addHalfEdge() {
    HalfEdge* last = new HalfEdge(*this);
    if (unusedHeids.size() == 0){
        last->setId(nHalfEdges);
        halfEdges.push_back(last);
        //halfEdgeLinks.push_back({-1, -1, -1, -1, -1, -1});
    }
    else {
        int heid = *(unusedHeids.begin());
        last->setId(heid);
        halfEdges[heid] = last;
        //halfEdgeLinks[heid] = {-1, -1, -1, -1, -1, -1};
        unusedHeids.erase(heid);
    }
    nHalfEdges++;
    return last;
}

/**
 * \~Italian
 * @brief Aggiunge una faccia alla Dcel e ne restituisce il puntatore.
 *
 * La Dcel::Face è inizializzata col costruttore vuoto, e successivamente viene settato un id
 * univoco.
 *
 * @return Il puntatore alla faccia appena inserita nella Dcel
 * @par Complessità:
 *      \e O(1)
 */
Dcel::Face* Dcel::addFace(const Vec3& n, const Color& c) {
    Face* last = new Face(*this);
    if (unusedFids.size() == 0){
        last->setId(nFaces);
        faces.push_back(last);
        faceNormals.push_back(n);
        faceColors.push_back(c);
    }
    else {
        int fid = *(unusedFids.begin());
        last->setId(fid);
        faces[fid] = last;
        faceNormals[fid] = n;
        faceColors[fid] = c;
        unusedFids.erase(fid);
    }
    nFaces++;
    return last;
}

/**
 * \~Italian
 * @brief Funzione che elimina il vertice passato in input.
 *
 * Una volta eliminato, il vertice passato in input non è più utilizzabile.
 *
 * Setta a nullptr tutti i seguenti campi se contengono un riferimento al vertice eliminato v:
 * per ogni half edge uscente he:
 * - he->fromVertex
 * - he->twin->toVertex
 *
 * @param[in] v: vertice da eliminare
 * @return True se il vertice è stato eliminato correttamente (ossia è stato trovato un vertice con
 * lo stesso id all'interno della lista di vertici), false altrimenti
 * @par Complessità:
 *      \e O(nIncidentHalfEdges) ->  \e ~O(1)
 */
bool Dcel::deleteVertex(Dcel::Vertex* v) {
    if (v->incidentHalfEdge != nullptr){
        Dcel::HalfEdge* he = v->incidentHalfEdge;
        do {
            if (he != nullptr){
                if (he->fromVertex == v) he->fromVertex = nullptr;
                if (he->twin != nullptr) {
                    he = he->twin;
                    if (he->toVertex == v) he->toVertex = nullptr;
                    he = he->next;
                }
                else he = v->incidentHalfEdge;
            }
            else he = v->incidentHalfEdge;
        } while (he != v->incidentHalfEdge);
    }
    vertices[v->id]=nullptr;
    unusedVids.insert(v->id);
    nVertices--;

    delete v;
    return true;
}

/**
 * \~Italian
 * @brief Funzione che elimina l'half edge passato in input.
 *
 * Una volta eliminato, il'half edge passato in input non è più utilizzabile.
 *
 * Setta a nullptr tutti i seguenti campi se contengono un riferimento all'half edge eliminato he:
 * - he->twin->twin
 * - he->next->prev
 * - he->prev->next
 * - he->fromVertex->incidentHalfEdge
 * - he->face->outerHalfEdge
 * eventuali inner half edge di he->face
 *
 * @param[in] he: half edge da eliminare
 * @return True se l'half edge è stato eliminato correttamente (ossia è stato trovato un half edge con
 * lo stesso id all'interno della lista degli half edge), false altrimenti
 * @par Complessità:
 *      \e O(1)
 */
bool Dcel::deleteHalfEdge(HalfEdge* he) {
    if (he->twin != nullptr)
        if (he->twin->twin == he) he->twin->twin = nullptr;
    if (he->next != nullptr)
        if (he->next->prev == he) he->next->prev = nullptr;
    if (he->prev != nullptr)
        if(he->prev->next == he) he->prev->next = nullptr;
    if (he->face != nullptr){
        if (he->face->outerHalfEdge == he) he->face->outerHalfEdge = nullptr;
        else if (he->face->hasHoles()) {
            for (unsigned int i = 0; i < he->face->innerHalfEdges.size(); i++){
                if (he->face->innerHalfEdges[i] == he)
                    he->face->innerHalfEdges.erase(he->face->innerHalfEdges.begin() + i--);
            }
        }
    }
    if (he->fromVertex != nullptr)
        if (he->fromVertex->incidentHalfEdge == he) he->fromVertex->incidentHalfEdge = nullptr;
    halfEdges[he->id] = nullptr;
    unusedHeids.insert(he->id);
    nHalfEdges--;

    delete he;
    return true;
}

/**
 * \~Italian
 * @brief Funzione che elimina la faccia passata in input.
 *
 * Una volta eliminata, la faccia passata in input non è più utilizzabile.
 *
 * Setta a nullptr tutti i seguenti campi se contengono un riferimento alla faccia eliminata f:
 * per ogni half edge incidente he:
 * - he->face
 *
 * @param[in] f: faccia da eliminare
 * @return True se la faccia è stata eliminata correttamente (ossia è stato trovata una faccia con
 * lo stesso id all'interno della lista delle facce), false altrimenti
 * @par Complessità:
 *      \e O(nIncidentHalfEdges) -> \e ~O(1)
 */
bool Dcel::deleteFace(Face* f) {
    if (f->outerHalfEdge != nullptr){
        Dcel::HalfEdge* he = f->outerHalfEdge;
        do {
            if (he->face == f) he->face = nullptr;
            he = he->next;
        } while (he != f->outerHalfEdge);
    }
    for (unsigned int i = 0; i < f->innerHalfEdges.size(); i++){
        Dcel::HalfEdge* he = f->innerHalfEdges[i];
        do {
            if (he->face == f) he->face = nullptr;
            he = he->next;
        } while (he != f->innerHalfEdges[i]);
    }
    faces[f->getId()]=nullptr;
    unusedFids.insert(f->getId());
    nFaces--;
    delete f;
    return true;
}

/**
 * \~Italian
 * @brief Funzione che ricalcola e aggiorna le normali delle facce presenti nella Dcel.
 *
 * Richiama il metodo \c updateNormal() della classe Dcel::Face, per cui non aggiorna
 * le normali delle facce che non sono triangoli.
 *
 * @warning Funziona solo sulle facce che sono triangoli
 * @warning Utilizza Dcel::Face::constIncidentVertexIterator
 * @par Complessità:
 *      \e O(numFaces)
 */
void Dcel::updateFaceNormals() {
    FaceIterator fit;
    for (fit = faceBegin(); fit != faceEnd(); ++fit){
        (*fit)->updateArea();
    }
}

/**
 * \~Italian
 * @brief Funzione che ricalcola e aggiorna le normali e le cardinalità dei vertici presenti nella Dcel.
 *
 * Richiama il metodo \c updateNormal() della classe Dcel::Vertex.
 *
 * @warning Utilizza Dcel::Vertex::ConstIncidentFaceIterator
 * @par Complessità:
 *      \e O(numVertices)
 */
void Dcel::updateVertexNormals() {
    VertexIterator vit;
    for (vit = vertexBegin(); vit != vertexEnd(); ++vit)
        (*vit)->updateNormal();
}

/**
 * \~Italian
 * @brief Funzione che ricalcola il bounding box più piccolo contenete la mesh
 * @return Il bounding box appena calcolato
 * @par Complessità:
 *      \e O(numVertices)
 */
BoundingBox Dcel::updateBoundingBox() {
    boundingBox.reset();
    for (ConstVertexIterator vit = vertexBegin(); vit!=vertexEnd(); ++vit){
        Pointd coord = (*vit)->getCoordinate();
        boundingBox.setMin(boundingBox.getMin().min(coord));
        boundingBox.setMax(boundingBox.getMax().max(coord));
    }
    return boundingBox;
}

void Dcel::setColor(const Color& c) {
    for (Dcel::Face* f : faceIterator())
        f->setColor(c);
}

void Dcel::scale(const BoundingBox& newBoundingBox) {
    Pointd oldCenter = boundingBox.center();
    Pointd newCenter = newBoundingBox.center();
    Pointd deltaOld = boundingBox.getMax() - boundingBox.getMin();
    Pointd deltaNew = newBoundingBox.getMax() - newBoundingBox.getMin();
    for (Dcel::VertexIterator vit = vertexBegin(); vit != vertexEnd(); ++vit) {
        Dcel::Vertex* v = *vit;
        v->setCoordinate(v->getCoordinate() - oldCenter);
        v->setCoordinate(v->getCoordinate() * (deltaNew / deltaOld));
        v->setCoordinate(v->getCoordinate() + newCenter);
    }
    boundingBox = newBoundingBox;
}

void Dcel::rotate(const Eigen::Matrix3d &matrix, const Pointd& centroid) {
    for (Dcel::VertexIterator vit = vertexBegin(); vit != vertexEnd(); ++vit){
        Dcel::Vertex* v = *vit;
        Pointd r = v->getCoordinate();
        r.rotate(matrix, centroid);
        v->setCoordinate(r);
    }
}

void Dcel::rotate(double matrix[3][3], const Pointd& centroid) {
    for (Dcel::VertexIterator vit = vertexBegin(); vit != vertexEnd(); ++vit){
        Dcel::Vertex* v = *vit;
        Pointd r = v->getCoordinate();
        r.rotate(matrix, centroid);
        v->setCoordinate(r);
    }
}

/**
 * \~Italian
 * @brief Funzione che ricalcola gli id di vertici, half edge e facce della Dcel.
 *
 * Ricompatta gli id all'interno delle liste contenute della Dcel.
 * Può essere utile richiamare questa funzione se sono state effettuate diverse cancellazioni
 * di componenti, e soprattutto se non si sono memorizzati riferimenti alle componenti mediante
 * vecchi id.
 *
 * @par Complessità:
 *      \e O(numVertices \e + \e NumHalfEdges \e + \e NumFaces)
 */
void Dcel::recalculateIds() {
    nVertices = 0;
    for (unsigned int i = 0; i < vertices.size(); i++){
        vertices[nVertices] = vertices[i];
        if (vertices[i] != nullptr) nVertices++;
    }
    unusedVids.clear();
    vertices.resize(nVertices);

    nHalfEdges = 0;
    for (unsigned int i = 0; i < halfEdges.size(); i++){
        halfEdges[nHalfEdges] = halfEdges[i];
        if (halfEdges[i] != nullptr) nHalfEdges++;
    }
    unusedHeids.clear();
    halfEdges.resize(nHalfEdges);

    nFaces = 0;
    for (unsigned int i = 0; i < faces.size(); i++){
        faces[nFaces] = faces[i];
        if (faces[i] != nullptr) nFaces++;
    }
    unusedFids.clear();
    faces.resize(nFaces);

}

/**
 * \~Italian
 * @brief Funzione che resetta i colori delle facce della Dcel.
 *
 * Setta ad ogni faccia un Color(), ossia il colore nero.
 *
 * @par Complessità:
 *      \e O(NumFaces)
 */
void Dcel::resetFaceColors() {
    for (FaceIterator fit = faceBegin(); fit != faceEnd(); ++fit) (*fit)->setColor(Color());
}

/**
 * \~Italian
 * @brief Funzione che cancella tutti i dati contenuti nella Dcel.
 *
 * Per cancellazione si intende la rimozione di tutti i vertici,
 * gli half-edge e le facce precedentemente create, con relativa
 * perdita di tutte le informazioni in esse contenute.
 *
 * @par Complessità:
 *      \e O(numVertices \e + \e NumHalfEdges \e + \e NumFaces)
 */
void Dcel::clear()	{
    for (unsigned int i=0; i<vertices.size(); i++)
        if (vertices[i] != nullptr)
            delete vertices[i];
    for (unsigned int i=0; i<halfEdges.size(); i++)
        if (halfEdges[i] != nullptr)
            delete halfEdges[i];
    for (unsigned int i=0; i<faces.size(); i++)
        if (faces[i] != nullptr)
            delete faces[i];
    vertices.clear();
    vertexCoordinates.clear();
    vertexNormals.clear();
    vertexColors.clear();
    halfEdges.clear();
    faces.clear();
    faceNormals.clear();
    unusedVids.clear();
    unusedHeids.clear();
    unusedFids.clear();
    nVertices = 0;
    nFaces = 0;
    nHalfEdges = 0;
}

#ifdef CGAL_DEFINED
/**
 * \~Italian
 * @brief Funzione che, presa in ingresso una faccia, ne crea una triangolazione.
 *
 * La faccia in ingresso diventa un triangolo (se se lo era già rimarrà un triangolo),
 * e inserisce nella Dcel tanti altri triangoli che comporranno la faccia triangolata.
 *
 * Utilizza CGAL.
 * Utilizzabile SOLAMENTE se è definita la costante letterale CGAL_DEFINED
 * @param[in] f: la faccia che verrà triangolata
 * @return Il numero di triangoli che compone la faccia appena triangolata.
 */
unsigned int Dcel::triangulateFace(Dcel::Face* f) {
    int count=0;
    if (f->isTriangle())
        return 1;
    else {
        // Taking all the coordinates on vectors
        Dcel::HalfEdge* firstHalfEdge = *(f->incidentHalfEdgeBegin());
        std::vector<Pointd> borderCoordinates;
        std::vector< std::vector<Pointd> > innerBorderCoordinates;
        std::map<std::pair<Dcel::Vertex*, Dcel::Vertex*> , Dcel::HalfEdge*> verticesEdgeMap;
        std::map<std::pair<Dcel::Vertex*, Dcel::Vertex*> , Dcel::HalfEdge*> twinsEdgeMap;
        std::map<Pointd, Dcel::Vertex*> pointsVerticesMap;
        for (Dcel::Face::IncidentHalfEdgeIterator heit = f->incidentHalfEdgeBegin(); heit != f->incidentHalfEdgeEnd(); ++heit){
            borderCoordinates.push_back((*heit)->getFromVertex()->getCoordinate());
            std::pair<Dcel::Vertex*, Dcel::Vertex*> pp;
            pp.first = (*heit)->getFromVertex();
            pp.second = (*heit)->getToVertex();
            verticesEdgeMap[pp] = *heit;
            pointsVerticesMap[(*heit)->getFromVertex()->getCoordinate()] = (*heit)->getFromVertex();
        }

        if (f->hasHoles()){
            innerBorderCoordinates.reserve(f->getNumberInnerHalfEdges());
            int i = 0;
            for (Dcel::Face::InnerHalfEdgeIterator ihe = f->innerHalfEdgeBegin(); ihe != f->innerHalfEdgeEnd(); ++ihe, ++i){
                Dcel::HalfEdge* he = *ihe;
                std::vector<Pointd> inner;
                for (Dcel::Face::IncidentHalfEdgeIterator heit = f->incidentHalfEdgeBegin(he); heit != f->incidentHalfEdgeEnd(); ++heit){
                    inner.push_back((*heit)->getFromVertex()->getCoordinate());
                    std::pair<Dcel::Vertex*, Dcel::Vertex*> pp;
                    pp.first = (*heit)->getFromVertex();
                    pp.second = (*heit)->getToVertex();
                    verticesEdgeMap[pp] = *heit;
                }
                innerBorderCoordinates.push_back(inner);
            }
        }

        ///TRIANGULATION

        std::vector<std::array<Pointd, 3> > triangulation = CGALInterface::Triangulation::triangulate(f->getNormal(), borderCoordinates, innerBorderCoordinates);
        ///


        ///RECONSTRUCTION
        for (unsigned int i = 0; i < triangulation.size(); i++) {
            std::array<Pointd, 3> triangle = triangulation[i];

                Pointd p1 = triangle[0];
                Pointd p2 = triangle[1];
                Pointd p3 = triangle[2];

                Dcel::HalfEdge* e1, *e2, *e3;
                std::pair<Dcel::Vertex*, Dcel::Vertex*> pp;
                bool b = false;
                pp.first = pointsVerticesMap[p1];
                pp.second = pointsVerticesMap[p2];
                if (verticesEdgeMap.find(pp) != verticesEdgeMap.end()){
                    e1 = verticesEdgeMap[pp];
                    if (e1 == firstHalfEdge) b = true;
                }
                else {
                    e1 = addHalfEdge();
                    e1->setFromVertex(pp.first);
                    e1->setToVertex(pp.second);
                    if (twinsEdgeMap.find(pp) == twinsEdgeMap.end()){
                        Dcel::Vertex* tmp = pp.first;
                        pp.first = pp.second;
                        pp.second = tmp;
                        twinsEdgeMap[pp] = e1;
                    }
                    else {
                        Dcel::HalfEdge* twin = twinsEdgeMap[pp];
                        twin->setTwin(e1);
                        e1->setTwin(twin);
                        twinsEdgeMap.erase(pp);
                    }
                }
                pp.first = pointsVerticesMap[p2];
                pp.second = pointsVerticesMap[p3];
                if (verticesEdgeMap.find(pp) != verticesEdgeMap.end()){
                    e2 = verticesEdgeMap[pp];
                    if (e2 == firstHalfEdge) b = true;
                }
                else {
                    e2 = addHalfEdge();
                    e2->setFromVertex(pp.first);
                    e2->setToVertex(pp.second);
                    if (twinsEdgeMap.find(pp) == twinsEdgeMap.end()){
                        Dcel::Vertex* tmp = pp.first;
                        pp.first = pp.second;
                        pp.second = tmp;
                        twinsEdgeMap[pp] = e2;
                    }
                    else {
                        Dcel::HalfEdge* twin = twinsEdgeMap[pp];
                        twin->setTwin(e2);
                        e2->setTwin(twin);
                        twinsEdgeMap.erase(pp);
                    }
                }
                pp.first = pointsVerticesMap[p3];
                pp.second = pointsVerticesMap[p1];
                if (verticesEdgeMap.find(pp) != verticesEdgeMap.end()){
                    e3 = verticesEdgeMap[pp];
                    if (e3 == firstHalfEdge) b = true;
                }
                else {
                    e3 = addHalfEdge();
                    e3->setFromVertex(pp.first);
                    e3->setToVertex(pp.second);
                    if (twinsEdgeMap.find(pp) == twinsEdgeMap.end()){
                        Dcel::Vertex* tmp = pp.first;
                        pp.first = pp.second;
                        pp.second = tmp;
                        twinsEdgeMap[pp] = e3;
                    }
                    else {
                        Dcel::HalfEdge* twin = twinsEdgeMap[pp];
                        twin->setTwin(e3);
                        e3->setTwin(twin);
                        twinsEdgeMap.erase(pp);
                    }
                }

                Dcel::Face* f;
                if (!b)
                    f = addFace();
                else
                    f = firstHalfEdge->getFace();

                e1->setNext(e2);
                e2->setNext(e3);
                e3->setNext(e1);
                e1->setPrev(e3);
                e2->setPrev(e1);
                e3->setPrev(e2);
                e1->setFace(f);
                e2->setFace(f);
                e3->setFace(f);
                f->setOuterHalfEdge(e1);
                f->setNormal(firstHalfEdge->getFace()->getNormal());
                f->setColor(firstHalfEdge->getFace()->getColor());

                ++count;
        }

    }
    return count;
}


/**
 * \~Italian
 * @brief Funzione che trasforma una Dcel composta da poligoni generici in una Dcel di triangoli.
 *
 * Per ogni faccia, ne viene creata una triangolazione. I triangoli presenti non vengono modificati.
 * Vengono tuttavia aggiornate le normali ai vertici.
 *
 * Utilizza CGAL.
 * Utilizzabile SOLAMENTE se è definita la costante letterale CGAL_DEFINED
 */
void Dcel::triangulate() {
    int i = 0;
    for (FaceIterator fit = faceBegin(); fit != faceEnd(); ++fit){
        Dcel::Face* f = *fit;
        triangulateFace(f);
        i++;
    }
    updateVertexNormals();
}
#endif

bool Dcel::loadFromFile(const std::string& filename) {
    std::string ext = filename.substr(filename.find_last_of(".") + 1);
    if(ext == "obj" || ext == "OBJ") { //obj file
        loadFromObjFile(filename);
        return true;
    }
    else if(ext == "ply" || ext == "PLY") { //ply file
        loadFromPlyFile(filename);
        return true;
    }
    else if (ext == "dcel" || ext == "dcel") {
        loadFromDcelFile(filename);
        return true;
    }
    else
        return false;
}


/**
 * \~Italian
 * @brief Funzione che carica una mesh salvta su file OBJ nella Dcel.
 *
 * Prima di caricare la mesh, viene chiamata la Dcel::reset().
 * Se, mediante il parametro regular, viene indicato che la mesh è chiusa e senza buchi (ossia
 * ogni vertice ha una \e star completa), una volta caricata la mesh, vengono anche calcolate
 * le normali ai vertici e le cardinalità.
 *
 * @param[in] filename: nome del file su cui è salvata la mesh, comprensivo di estensione
 * @param[in] regular: se true (default), indica che la mesh è chiusa e priva di buchi
 * @warning Se regular, utilizza Dcel::Vertex::ConstIncidentFaceIterator
 * @return Una stringa indicante da quanti vertici, half edge e facce è composta la mesh caricata
 */
std::string Dcel::loadFromObjFile(const std::string& filename, bool regular) {
    clear();
    typedef boost::char_separator<char>     CharSeparator;
    typedef boost::tokenizer<CharSeparator> Tokenizer;
    typedef Tokenizer::iterator             TokenizerIterator;

    CharSeparator spaceSeparator(" ");
    CharSeparator slashSeparator("/");

    static std::string vertex = "v";
    static std::string face   = "f";

    std::ifstream file(filename.c_str());
    std::string   line;

    std::vector<Vertex*> vertices;

    std::map< std::pair<int,int>, HalfEdge* > edge;
    std::map< std::pair<int,int>, HalfEdge* >::iterator eiter;

    bool first = true;

    if(!file.is_open())
    {
        std::cerr << "ERROR : read() : could not open input file " << filename.c_str() << "\n";
        assert(0);
    }

    while(std::getline(file,line))
    {
        Tokenizer spaceTokenizer(line, spaceSeparator);

        if (spaceTokenizer.begin() == spaceTokenizer.end()) continue;

        TokenizerIterator token = spaceTokenizer.begin();
        std::string header = *token;

        // Handle
        //
        // v 0.123 0.234 0.345
        // v 0.123 0.234 0.345 1.0

        if (strcmp(header.c_str(), vertex.c_str()) == 0)
        {
            std::string x = *(++token);
            std::string y = *(++token);
            std::string z = *(++token);

            std::istringstream xstr(x), ystr(y), zstr(z);
            double  xd, yd, zd;
            xstr >> xd;
            ystr >> yd;
            zstr >> zd;

            Pointd coord(xd, yd, zd);

            if (first) {
                boundingBox.setMin(coord);
                boundingBox.setMax(coord);
                first = false;
            }

            if (coord.x() < boundingBox.getMinX()) boundingBox.setMinX(coord.x());
            if (coord.y() < boundingBox.getMinY()) boundingBox.setMinY(coord.y());
            if (coord.z() < boundingBox.getMinZ()) boundingBox.setMinZ(coord.z());

            if (coord.x() > boundingBox.getMaxX()) boundingBox.setMaxX(coord.x());
            if (coord.y() > boundingBox.getMaxY()) boundingBox.setMaxY(coord.y());
            if (coord.z() > boundingBox.getMaxZ()) boundingBox.setMaxZ(coord.z());

            Vertex* vid = addVertex(coord);

            vertices.push_back(vid);
        }

        // Handle
        //
        // f 1 2 3
        // f 3/1 4/2 5/3
        // f 6/4/1 3/5/3 7/6/5

        else if (strcmp(header.c_str(), face.c_str()) == 0)
        {
            std::vector<std::string> dummy;
            int i=-1;
            token++;
            while (token != spaceTokenizer.end()) {
                dummy.push_back(*(token));
                token++;
                i++;
            }

            std::vector<Tokenizer> slashTokenizer;
            for (unsigned int i=0; i<dummy.size(); i++){
                Tokenizer t(dummy[i], slashSeparator);
                slashTokenizer.push_back(t);
            }

            std::vector<int> nid;
            for (unsigned int i=0; i<slashTokenizer.size(); i++){
                std::istringstream istr((*slashTokenizer[i].begin()).c_str());
                int id;
                istr >> id;
                nid.push_back(id);
            }
            nid.push_back(nid[0]);
            std::pair<int, int> p;
            HalfEdge* eid = nullptr;
            Face* fid = addFace();
            HalfEdge* prev = nullptr;
            HalfEdge* first = nullptr;

            for (unsigned int i=0; i<nid.size()-1; i++){
                eid = addHalfEdge();
                if (i==0) {
                    first = eid;
                    fid->setOuterHalfEdge(eid);
                }
                else {
                    eid->setPrev(prev);
                    prev->setNext(eid);
                }
                vertices[nid[i] - 1]->setIncidentHalfEdge(eid);
                eid->setFromVertex(vertices[nid[i] - 1]);
                vertices[nid[i] - 1]->incrementCardinality();
                eid->setToVertex(vertices[nid[i+1] - 1]);
                eid->setFace(fid);
                p.first = nid[i+1] - 1;
                p.second = nid[i] - 1;
                eiter = edge.find(p);
                if (eiter != edge.end()){
                    HalfEdge* twin = edge[p];
                    eid->setTwin(twin);
                    twin->setTwin(eid);
                    edge.erase(eiter);
                }
                else {
                    p.first = nid[i] - 1;
                    p.second = nid[i+1] - 1;
                    edge[p] = eid;
                }
                prev = eid;
            }
            eid->setNext(first);
            first->setPrev(eid);

            fid->setColor(Color(128, 128, 128));

            fid->updateNormal();
            fid->updateArea();
        }

        // Ignore
        //
        // vt 0.500 1 [0]
        // vn 0.707 0.000 0.707
        // vp 0.310000 3.210000 2.100000
        // ...

    }
    file.close();
    if (regular) updateVertexNormals();
    std::stringstream ss;
    ss << "Vertices: " << nVertices << "; Half Edges: " << nHalfEdges << "; Faces: " << nFaces << ".";
    return ss.str();
}

/**
 * \~Italian
 * @brief Funzione che carica una mesh salvta su file PLY nella Dcel.
 *
 * Prima di caricare la mesh, viene chiamata la Dcel::reset().
 * Se, mediante il parametro regular, viene indicato che la mesh è chiusa e senza buchi (ossia
 * ogni vertice ha una \e star completa), una volta caricata la mesh, vengono anche calcolate
 * le normali ai vertici e le cardinalità.
 *
 * @param[in] filename: nome del file su cui è salvata la mesh, comprensivo di estensione
 * @param[in] regular: se true (default), indica che la mesh è chiusa e priva di buchi
 * @warning Se regular, utilizza Dcel::Vertex::ConstIncidentFaceIterator
 * @todo Gestione colori vertici
 * @return Una stringa indicante da quanti vertici, half edge e facce è composta la mesh caricata
 */
std::string Dcel::loadFromPlyFile(const std::string& filename, bool regular) {
    clear();
    typedef boost::char_separator<char>     CharSeparator;
    typedef boost::tokenizer<CharSeparator> Tokenizer;
    typedef Tokenizer::iterator             TokenizerIterator;

    CharSeparator spaceSeparator(" ");

    std::ifstream file(filename.c_str());
    std::string   line;

    std::vector<Vertex*> vertices;

    int nVer = -1, nFac = -1;
    int nv=0, nf=0;

    bool first = true;

    std::map< std::pair<int,int>, HalfEdge* > edgeMap;
    std::map< std::pair<int,int>, HalfEdge* >::iterator eiter;

    if(!file.is_open())
    {
        std::cerr << "ERROR : read() : could not open input file " << filename.c_str() << "\n";
        assert(0);
    }

    bool header = true, vertex=false, face=false, edge=false, vertex_color_int = false, face_color_int = false;
    std::pair<std::string, std::string> vertexOrder("", ""), faceOrder("", "");
    while(std::getline(file,line))
    {
        Tokenizer spaceTokenizer(line, spaceSeparator);

        if (spaceTokenizer.begin() == spaceTokenizer.end()) continue;

        TokenizerIterator token = spaceTokenizer.begin();
        std::string headerLine = *token;
        if (header){
            if (headerLine == "end_header"){
                header = false;
            }
            if (headerLine == "element"){
                std::string s = *(++token);
                if (s == "vertex"){
                    std::istringstream nstr((*(++token)).c_str());
                    nstr >> nVer;
                    vertex = true;
                    face = edge = false;
                }
                if (s == "face"){
                    std::istringstream nstr((*(++token)).c_str());
                    nstr >> nFac;
                    face = true;
                    vertex = edge = false;
                }
            }
            if (headerLine == "property"){
                std::string type;
                if (vertex){
                    type = *(++token);
                    ++token;
                    if (*token == "red"){
                        if (vertexOrder.first == "") vertexOrder.first = "color";
                        else vertexOrder.second = "color";
                        if (type == "uchar" || type == "int") vertex_color_int = true;
                    }
                    if (*token == "nx") {
                        if (vertexOrder.first == "") vertexOrder.first = "normal";
                        else vertexOrder.second = "normal";
                    }
                }
                if (face){
                    type = *(++token);
                    ++token;
                    if (*token == "red"){
                        if (faceOrder.first == "") faceOrder.first = "color";
                        else faceOrder.second = "color";
                        if (type == "uchar" || type == "int") face_color_int = true;
                    }
                    if (*token == "nx") {
                        if (faceOrder.first == "") faceOrder.first = "normal";
                        else faceOrder.second = "normal";
                    }

                }
            }
        }
        else {
            if (nv < nVer){
                std::string x = *(token);
                std::string y = *(++token);
                std::string z = *(++token);

                std::istringstream xstr(x), ystr(y), zstr(z);
                double  xd, yd, zd;
                xstr >> xd;
                ystr >> yd;
                zstr >> zd;

                Pointd coord(xd, yd, zd);

                if (first) {
                    boundingBox.setMin(coord);
                    boundingBox.setMax(coord);
                    first = false;
                }

                if (coord.x() < boundingBox.getMinX()) boundingBox.setMinX(coord.x());
                if (coord.y() < boundingBox.getMinY()) boundingBox.setMinY(coord.y());
                if (coord.z() < boundingBox.getMinZ()) boundingBox.setMinZ(coord.z());

                if (coord.x() > boundingBox.getMaxX()) boundingBox.setMaxX(coord.x());
                if (coord.y() > boundingBox.getMaxY()) boundingBox.setMaxY(coord.y());
                if (coord.z() > boundingBox.getMaxZ()) boundingBox.setMaxZ(coord.z());

                Vertex* vid = addVertex(coord);
                nv++;

                vertices.push_back(vid);
                if (vertexOrder.first == "normal"){
                    std::string x = *(++token);
                    std::string y = *(++token);
                    std::string z = *(++token);

                    std::istringstream xstr(x), ystr(y), zstr(z);
                    double  xd, yd, zd;
                    xstr >> xd;
                    ystr >> yd;
                    zstr >> zd;

                    Vec3 norm(xd, yd, zd);
                    vid->setNormal(norm);
                }
                if (vertexOrder.first == "color"){
                    std::string x = *(++token);
                    std::string y = *(++token);
                    std::string z = *(++token);

                    std::istringstream xstr(x), ystr(y), zstr(z);

                    if (vertex_color_int){
                        int  xd, yd, zd;
                        xstr >> xd;
                        ystr >> yd;
                        zstr >> zd;
                        Color color(xd, yd, zd);
                        //vid->set_color(color);
                    }
                    else {
                        float  xd, yd, zd;
                        xstr >> xd;
                        ystr >> yd;
                        zstr >> zd;
                        Color color;
                        color.setRgbF(xd, yd, zd);
                        //vid->set_color(color);
                    }
                }
                if (vertexOrder.second == "normal"){
                    std::string x = *(++token);
                    std::string y = *(++token);
                    std::string z = *(++token);

                    std::istringstream xstr(x), ystr(y), zstr(z);
                    double  xd, yd, zd;
                    xstr >> xd;
                    ystr >> yd;
                    zstr >> zd;

                    Vec3 norm(xd, yd, zd);
                    vid->setNormal(norm);
                }
                if (vertexOrder.second == "color"){
                    std::string x = *(++token);
                    std::string y = *(++token);
                    std::string z = *(++token);

                    std::istringstream xstr(x), ystr(y), zstr(z);

                    if (vertex_color_int){
                        int  xd, yd, zd;
                        xstr >> xd;
                        ystr >> yd;
                        zstr >> zd;
                        Color color(xd, yd, zd);
                        //vid->set_color(color);
                    }
                    else {
                        float  xd, yd, zd;
                        xstr >> xd;
                        ystr >> yd;
                        zstr >> zd;
                        Color color;
                        color.setRgbF(xd, yd, zd);
                        //vid->set_color(color);
                    }
                }

            }
            else {
                if (nf < nFac){
                    std::vector<std::string> dummy;
                    std::istringstream nstr((*(token++)).c_str());
                    int nvf;
                    nstr >> nvf;
                    while (token != spaceTokenizer.end()) {
                        dummy.push_back(*(token++));
                    }

                    std::vector<int> nid;
                    for (int i=0; i<nvf; i++){
                        std::istringstream nstr((dummy[i]).c_str());
                        int nf;
                        nstr >> nf;
                        nid.push_back(nf);
                    }
                    nid.push_back(nid[0]);
                    std::pair<int, int> p;
                    HalfEdge* eid = nullptr;
                    Face* fid = addFace();
                    HalfEdge* prev = nullptr;
                    HalfEdge* first = nullptr;

                    for (unsigned int i=0; i<nid.size()-1; i++){
                        eid = addHalfEdge();
                        if (i==0) {
                            first = eid;
                            fid->setOuterHalfEdge(eid);
                        }
                        else {
                            eid->setPrev(prev);
                            prev->setNext(eid);
                        }
                        vertices[nid[i]]->setIncidentHalfEdge(eid);
                        eid->setFromVertex(vertices[nid[i]]);
                        vertices[nid[i]]->incrementCardinality();
                        eid->setToVertex(vertices[nid[i+1]]);
                        eid->setFace(fid);
                        p.first = nid[i+1];
                        p.second = nid[i];
                        eiter = edgeMap.find(p);
                        if (eiter != edgeMap.end()){
                            HalfEdge* twin = edgeMap[p];
                            eid->setTwin(twin);
                            twin->setTwin(eid);
                            edgeMap.erase(eiter);
                        }
                        else {
                            p.first = nid[i];
                            p.second = nid[i+1];
                            edgeMap[p] = eid;
                        }
                        prev = eid;
                    }
                    eid->setNext(first);
                    first->setPrev(eid);

                    fid->updateNormal();
                    fid->updateArea();
                    fid->setColor(Color(128, 128, 128));
                    nf++;
                    if (faceOrder.first == "normal"){
                        std::string x = dummy[nvf];
                        std::string y = dummy[nvf+1];
                        std::string z = dummy[nvf+2];

                        std::istringstream xstr(x), ystr(y), zstr(z);
                        double  xd, yd, zd;
                        xstr >> xd;
                        ystr >> yd;
                        zstr >> zd;

                        Vec3 norm(xd, yd, zd);
                        fid->setNormal(norm);
                    }
                    if (faceOrder.first == "color"){
                        std::string x = dummy[nvf];
                        std::string y = dummy[nvf+1];
                        std::string z = dummy[nvf+2];

                        std::istringstream xstr(x), ystr(y), zstr(z);

                        if (face_color_int){
                            int  xd, yd, zd;
                            xstr >> xd;
                            ystr >> yd;
                            zstr >> zd;
                            Color color(xd, yd, zd);
                            fid->setColor(color);
                        }
                        else {
                            float  xd, yd, zd;
                            xstr >> xd;
                            ystr >> yd;
                            zstr >> zd;
                            Color color;
                            color.setRgbF(xd, yd, zd);
                            fid->setColor(color);
                        }
                    }
                    if (faceOrder.second == "normal"){
                        std::string x = dummy[nvf+3];
                        std::string y = dummy[nvf+4];
                        std::string z = dummy[nvf+5];

                        std::istringstream xstr(x), ystr(y), zstr(z);
                        double  xd, yd, zd;
                        xstr >> xd;
                        ystr >> yd;
                        zstr >> zd;

                        Vec3 norm(xd, yd, zd);
                        fid->setNormal(norm);
                    }
                    if (faceOrder.second == "color"){
                        std::string x = dummy[nvf];
                        std::string y = dummy[nvf+1];
                        std::string z = dummy[nvf+2];

                        std::istringstream xstr(x), ystr(y), zstr(z);

                        if (face_color_int){
                            int  xd, yd, zd;
                            xstr >> xd;
                            ystr >> yd;
                            zstr >> zd;
                            Color color(xd, yd, zd);
                            fid->setColor(color);
                        }
                        else {
                            float  xd, yd, zd;
                            xstr >> xd;
                            ystr >> yd;
                            zstr >> zd;
                            Color color;
                            color.setRgbF(xd, yd, zd);
                            fid->setColor(color);
                        }
                    }

                }
            }
        }
    }
    file.close();
    if (regular) updateVertexNormals();
    std::stringstream ss;
    ss << "Vertices: " << nVertices << "; Half Edges: " << nHalfEdges << "; Faces: " << nFaces << ".";
    return ss.str();
}


/**
 * \~Italian
 * @brief Funzione che carica una mesh salvta su file DCEL nella Dcel.
 *
 * Prima di caricare la mesh, viene chiamata la Dcel::reset() (solo se il file esiste).
 *
 * @param[in] filename: nome del file su cui è salvata la mesh, comprensivo di estensione
 * @todo Gestione colori vertici
 * @return Una stringa indicante da quanti vertici, half edge e facce è composta la mesh caricata
 */
std::string Dcel::loadFromDcelFile(const std::string& filename) {
    std::ifstream myfile;
    myfile.open (filename, std::ios::in | std::ios::binary);
    if (myfile.is_open()) {
        deserialize(myfile);
    }
    myfile.close();

    std::stringstream ss;
    ss << "Vertices: " << nVertices << "; Half Edges: " << nHalfEdges << "; Faces: " << nFaces << ".";
    return ss.str();
}

/**
 * \~Italian
 * @brief Dcel::deserialize
 *
 * Deserializza la dcel da uno std::ifstream già aperto in modalità bin, che deve puntare necessariamente
 * al primo blocco contenente i dati della dcel.
 * Lo stream passato in input deve essere già aperto, e a fine funzione punterà al blocco immediatamente successivo
 * all'ultima informazione della dcel salvata presente nel file.
 * Può essere quindi utilizzata per caricare una dcel contenuta in un file binario insieme ad altri dati.
 * Per salvare la dcel in un file binario insieme ad altri dati, vedere la Dcel::serialize().
 *
 * @param[in] binaryFile : std::ifstream del file binario da dove si vuole caricare.
 */
bool Dcel::deserialize(std::ifstream &binaryFile) {
    Dcel tmp;
    //BB

    if (! tmp.boundingBox.deserialize(binaryFile)) return false;
    //N
    if (! Serializer::deserialize(tmp.nVertices, binaryFile)) return false;
    if (! Serializer::deserialize(tmp.nHalfEdges, binaryFile)) return false;
    if (! Serializer::deserialize(tmp.nFaces, binaryFile)) return false;

    if (! Serializer::deserialize(tmp.unusedVids, binaryFile)) return false;
    if (! Serializer::deserialize(tmp.unusedHeids, binaryFile)) return false;
    if (! Serializer::deserialize(tmp.unusedFids, binaryFile)) return false;

    //Vertices
    tmp.vertices.resize(tmp.nVertices+tmp.unusedVids.size(), nullptr);
    tmp.vertexCoordinates.resize(tmp.nVertices+tmp.unusedVids.size(), Pointd());
    tmp.vertexNormals.resize(tmp.nVertices+tmp.unusedVids.size(), Vec3());
    tmp.vertexColors.resize(tmp.nVertices+tmp.unusedVids.size(), Color());
    std::map<int, int> vert;

    for (unsigned int i = 0; i < tmp.nVertices; i++){
        int id, heid;
        Pointd coord; Vec3 norm;
        int c, f;
        if (! Serializer::deserialize(id, binaryFile)) return false;
        if (! coord.deserialize(binaryFile)) return false;
        if (! norm.deserialize(binaryFile)) return false;
        if (! Serializer::deserialize(heid, binaryFile)) return false;
        if (! Serializer::deserialize(c, binaryFile)) return false;
        if (! Serializer::deserialize(f, binaryFile)) return false;

        Dcel::Vertex* v = tmp.addVertex(id);
        v->setCardinality(c);
        v->setCoordinate(coord);
        v->setNormal(norm);
        v->setFlag(f);
        vert[id] = heid;
    }
    //HalfEdges
    tmp.halfEdges.resize(tmp.nHalfEdges+tmp.unusedHeids.size(), nullptr);
    std::map<int, std::array<int, 6> > edges;

    for (unsigned int i = 0; i < tmp.nHalfEdges; i++){
        int id, fv, tv, tw, prev, next, face, flag;
        if (! Serializer::deserialize(id, binaryFile)) return false;
        if (! Serializer::deserialize(fv, binaryFile)) return false;
        if (! Serializer::deserialize(tv, binaryFile)) return false;
        if (! Serializer::deserialize(tw, binaryFile)) return false;
        if (! Serializer::deserialize(prev, binaryFile)) return false;
        if (! Serializer::deserialize(next, binaryFile)) return false;
        if (! Serializer::deserialize(face, binaryFile)) return false;
        if (! Serializer::deserialize(flag, binaryFile)) return false;
        Dcel::HalfEdge* he = tmp.addHalfEdge(id);
        he->setFlag(flag);
        edges[id] = {fv, tv, tw, prev, next, face};
    }

    //Faces
    tmp.faces.resize(tmp.nFaces+tmp.unusedFids.size(), nullptr);
    tmp.faceNormals.resize(tmp.nFaces+tmp.unusedFids.size(), Vec3());
    tmp.faceColors.resize(tmp.nFaces+tmp.unusedFids.size(), Color());
    for (unsigned int i = 0; i < tmp.nFaces; i++){
        int id, ohe, /*cr, cg, cb,*/ flag, nihe;
        double /*nx, ny, nz,*/ area;
        Color color;
        Vec3 norm;
        if (! Serializer::deserialize(id, binaryFile)) return false;
        if (! Serializer::deserialize(ohe, binaryFile)) return false;
        if (! norm.deserialize(binaryFile)) return false;
        if (! Serializer::deserialize(color, binaryFile)) return false;
        if (! Serializer::deserialize(area, binaryFile)) return false;
        if (! Serializer::deserialize(flag, binaryFile)) return false;
        if (! Serializer::deserialize(nihe, binaryFile)) return false;


        Dcel::Face* f = tmp.addFace(id);
        f->setColor(color);
        f->setNormal(norm);
        f->setArea(area);
        f->setFlag(flag);
        f->setOuterHalfEdge(tmp.getHalfEdge(ohe));
        for (int j = 0; j < nihe; j++){
            int idhe;
            if (! Serializer::deserialize(idhe, binaryFile)) return false;
            f->addInnerHalfEdge(tmp.getHalfEdge(idhe));
        }
    }

    for (VertexIterator vit = tmp.vertexBegin(); vit != tmp.vertexEnd(); ++vit){
        Dcel::Vertex* v = *vit;
        v->setIncidentHalfEdge(tmp.getHalfEdge(vert[v->getId()]));
    }
    for (HalfEdgeIterator heit = tmp.halfEdgeBegin(); heit != tmp.halfEdgeEnd(); ++heit){
        Dcel::HalfEdge* he = *heit;
        std::array<int, 6> a = edges[he->getId()];
        he->setFromVertex(tmp.getVertex(a[0]));
        he->setToVertex(tmp.getVertex(a[1]));
        he->setTwin(tmp.getHalfEdge(a[2]));
        he->setPrev(tmp.getHalfEdge(a[3]));
        he->setNext(tmp.getHalfEdge(a[4]));
        he->setFace(tmp.getFace(a[5]));
    }

    *this = std::move(tmp);
    return true;
}

/**
 * \~Italian
 * @brief Operatore di assegnamento della Dcel.
 *
 * Assegna alla Dcel this un'altra Dcel dcel.
 * Elimina la dcel precedentemente contenuta e successivamente ne fa una copia, che funziona
 * esattamente come il costruttore di copia Dcel(Dcel d):
 * crea una nuova Dcel a partire dalla Dcel passata in input, creando nuove istanze
 * di tutti i vertici, gli half edge e le facce contenute in dcel, e settando di conseguenza
 * tutte le relazioni tra essi.
 * @param[in] dcel: dcel da cui verrà creata la Dcel this
 * @return La Dcel appena assegnata
 */
Dcel& Dcel::operator = (const Dcel& dcel) {
    this->clear();
    vertices.clear();
    halfEdges.clear();
    faces.clear();
    copyFrom(dcel);
    return *this;
}

/*Dcel& Dcel::operator=(Dcel&& dcel) {
    std::swap(vertices, dcel.vertices);
    std::swap(halfEdges, dcel.halfEdges);
    std::swap(faces, dcel.faces);
    std::swap(unusedVids, dcel.unusedVids);
    std::swap(unusedHeids, dcel.unusedHeids);
    std::swap(unusedFids, dcel.unusedFids);
    std::swap(nVertices, dcel.nVertices);
    std::swap(nHalfEdges, dcel.nHalfEdges);
    std::swap(nFaces, dcel.nFaces);
    std::swap(boundingBox, dcel.boundingBox);
    return *this;
}*/

/*********************
 * Protected Methods *
 ********************/

/**
 * \~Italian
 * @brief Aggiunge un vertice avente l'id passato come parametro alla Dcel e ne restituisce il puntatore.
 *
 * Il Dcel::Vertex è inizializzato col costruttore vuoto, e viene settato l'id passato come parametro.
 *
 * @param[in] id: l'id del vertice che verrà creato.
 * @return Il puntatore al vertice appena inserito nella Dcel
 */
Dcel::Vertex*Dcel::addVertex(int id) {
    Vertex* last= new Vertex(*this);
    last->setId(id);
    vertices[id] = last;
    return last;
}

/**
 * \~Italian
 * @brief Aggiunge un half edge avente l'id passato come parametro alla Dcel e ne restituisce il puntatore.
 *
 * Il Dcel::HalfEdge è inizializzato col costruttore vuoto, e viene settato l'id passato come parametro.
 *
 * @param[in] id: l'id dell'half edge che verrà creato.
 * @return Il puntatore all'half edge appena inserito nella Dcel
 */
Dcel::HalfEdge*Dcel::addHalfEdge(int id) {
    HalfEdge* last = new HalfEdge(*this);
    last->setId(id);
    halfEdges[id] = last;
    return last;
}

/**
 * \~Italian
 * @brief Aggiunge una faccia avente l'id passato come parametro alla Dcel e ne restituisce il puntatore.
 *
 * La Dcel::Face è inizializzata col costruttore vuoto, e viene settato l'id passato come parametro.
 *
 * @param[in] id: l'id della faccia che verrà creata.
 * @return Il puntatore alla faccia appena inserita nella Dcel
 */
Dcel::Face*Dcel::addFace(int id) {
    Face* last = new Face(*this);
    last->setId(id);
    faces[id] = last;
    return last;
}

/**
 * \~Italian
 * @brief Funzione che, data in ingresso una faccia avente dei buchi, restituisce una singola lista di vertici di una faccia avente dummy edge.
 * @param[in] f: faccia avente almeno un buco
 * @return Lista di vertice rappresentante una faccia senza buchi ma con dummy edge
 */
std::vector<const Dcel::Vertex*> Dcel::makeSingleBorder(const Face* f) const {
    std::vector< std::vector<const Vertex*> > visited;
    std::vector< std::vector<const Vertex*> > notVisited;
    std::map< std::pair<int, int>, std::pair<int, int> > links;
    std::vector<const Vertex*> tmp;
    for (Face::ConstIncidentHalfEdgeIterator heit = f->incidentHalfEdgeBegin(); heit != f->incidentHalfEdgeEnd(); ++heit){
        #ifdef DEBUG
        (*heit)->checkFromVertex();
        #endif
        tmp.push_back((*heit)->getFromVertex());
    }
    visited.push_back(tmp);
    for (Face::ConstInnerHalfEdgeIterator iheit = f->innerHalfEdgeBegin(); iheit != f->innerHalfEdgeEnd(); ++iheit){
        tmp.clear();
        for (Face::ConstIncidentHalfEdgeIterator heit = f->incidentHalfEdgeBegin(*iheit); heit != f->incidentHalfEdgeEnd(); ++heit){
            #ifdef DEBUG
            (*heit)->checkFromVertex();
            #endif
            tmp.push_back((*heit)->getFromVertex());
        }
        notVisited.push_back(tmp);
    }
    while (notVisited.size() > 0) {
        // trova la coppia di punti tra visited e not_visited più vicina
        double dist = FLT_MAX;
        int visIndex = -1;
        int notvisIndex = -1;
        int visI = -1;
        int visJ = -1;
        for (unsigned int i=0; i<visited.size(); ++i){
            std::vector<const Vertex*> tmpVisited = visited[i];
            for (unsigned int j=0; j<notVisited.size(); ++j){
                std::vector<const Vertex*> tmpNotVisited = notVisited[j];
                for (unsigned int vi = 0; vi<tmpVisited.size(); ++vi){
                    for (unsigned int vj = 0; vj<tmpNotVisited.size(); ++vj){
                        if (((tmpVisited[vi])->dist(tmpNotVisited[vj])) < dist){
                            visIndex = vi;
                            notvisIndex = vj;
                            visI = i;
                            visJ = j;
                            dist = (tmpVisited[vi])->dist(tmpNotVisited[vj]);
                        }
                    }
                }
            }
        }
        std::pair<int, int> p1;
        p1.first = visI;
        p1.second = visIndex;
        std::pair<int, int> p2;
        p2.first = visited.size();
        p2.second = notvisIndex;
        visited.push_back(notVisited[visJ]);
        notVisited.erase(notVisited.begin() + visJ);
        links[p1] = p2;
        links[p2] = p1;
    }
    std::vector<const Vertex*> border;
    const Vertex* start = visited[0][0];
    border.push_back(start);
    const Vertex* v = nullptr;
    int i = 0;
    int j = 0;
    while (v != start){
        std::pair<int, int> p;
        p.first = i;
        p.second = j;
        if (links.find(p) != links.end()){
            p = links[p];
            i = p.first;
            j = p.second;
            border.push_back(visited[i][j]);
        }
        j = (j + 1) % (visited[i]).size();
        border.push_back(visited[i][j]);
        v = visited[i][j];
    }
    return border;
}

void Dcel::toStdVectors(std::vector<double>& vertices, std::vector<double>& verticesNormals, std::vector<int>& faces, std::vector<unsigned int>& faceSizes, std::vector<float>& faceColors) const{
    std::map<int, int> mapVertices;
    vertices.reserve(getNumberVertices()*3);
    verticesNormals.reserve(getNumberVertices()*3);
    faces.reserve(getNumberFaces()*3);
    faceSizes.reserve(getNumberFaces());
    faceColors.reserve(getNumberFaces()*3);

    int iv = 0;
    for (const Dcel::Vertex* v : vertexIterator()){
        vertices.push_back(v->getCoordinate().x());
        vertices.push_back(v->getCoordinate().y());
        vertices.push_back(v->getCoordinate().z());
        verticesNormals.push_back(v->getNormal().x());
        verticesNormals.push_back(v->getNormal().y());
        verticesNormals.push_back(v->getNormal().z());
        mapVertices[v->getId()] = iv++;
    }
    for (const Dcel::Face* f : faceIterator()){
        int size = 0;
        if (f->getNumberInnerHalfEdges() == 0) {
            for (const Dcel::Vertex* v : f->incidentVertexIterator()){
                assert(mapVertices.find(v->getId()) != mapVertices.end());
                faces.push_back(mapVertices[v->getId()]);
                size++;
            }

        }
        else { // holes
            std::vector<const Vertex*> v = makeSingleBorder(f);
            for (unsigned int i = 0; i<v.size(); ++i) {
                assert(mapVertices.find(v[i]->getId()) != mapVertices.end());
                faces.push_back(mapVertices[v[i]->getId()]);
            }
            size = v.size();
        }
        faceSizes.push_back(size);
        faceColors.push_back(f->getColor().redF());
        faceColors.push_back(f->getColor().greenF());
        faceColors.push_back(f->getColor().blueF());
    }
}

/**
 * \~Italian
 * @brief Funzione che esegue una Deep Copy della Dcel passata in input nella Dcel this.
 *
 * Nessuno degli elementi contenuti nella Dcel d viene copiato per riferimento ma per valore,
 * e vengono ricostruite tutte le relazioni tra i nuovi elementi.
 * Vengono preservati gli id degli elementi.
 *
 * @param d: Dcel da cui verrà fatta la copia
 */
void Dcel::copyFrom(const Dcel &d) {
    this->unusedVids = d.unusedVids;
    this->unusedHeids = d.unusedHeids;
    this->unusedFids = d.unusedFids;
    this->nVertices = d.nVertices;
    this->nHalfEdges = d.nHalfEdges;
    this->nFaces = d.nFaces;
    this->boundingBox = d.boundingBox;
    std::map<const Dcel::Vertex*, Dcel::Vertex*> mapVertices;
    std::map<const Dcel::HalfEdge*, Dcel::HalfEdge*> mapHalfEdges;
    std::map<const Dcel::Face*, Dcel::Face*> mapFaces;
    this->vertices.resize(d.vertices.size(), nullptr);
    this->vertexCoordinates.resize(d.vertexCoordinates.size(), Pointd());
    this->vertexNormals.resize(d.vertexNormals.size(), Vec3());
    this->vertexColors.resize(d.vertexColors.size(), Color());
    for (Dcel::ConstVertexIterator vit = d.vertexBegin(); vit != d.vertexEnd(); ++vit) {
        const Dcel::Vertex* ov = *vit;
        Dcel::Vertex* v = this->addVertex(ov->getId());
        v->setId(ov->getId());
        v->setCoordinate(ov->getCoordinate());
        v->setFlag(ov->getFlag());
        v->setCardinality(ov->getCardinality());
        v->setNormal(ov->getNormal());
        v->setColor(ov->getColor());
        mapVertices[ov] = v;
    }

    this->halfEdges.resize(d.halfEdges.size(), nullptr);
    //this->halfEdgeLinks.resize(d.halfEdgeLinks.size(), {-1, -1, -1, -1, -1, -1});
    for (Dcel::ConstHalfEdgeIterator heit = d.halfEdgeBegin(); heit != d.halfEdgeEnd(); ++heit) {
        const Dcel::HalfEdge* ohe = *heit;
        Dcel::HalfEdge* he = this->addHalfEdge(ohe->getId());
        he->setId(ohe->getId());
        he->setFlag(ohe->getFlag());
        he->setFromVertex(mapVertices[ohe->getFromVertex()]);
        he->setToVertex(mapVertices[ohe->getToVertex()]);
        mapHalfEdges[ohe] = he;
    }

    this->faces.resize(d.faces.size(), nullptr);
    this->faceNormals.resize(d.faceNormals.size(), Vec3());
    this->faceColors.resize(d.faceColors.size(), Color());
    for (Dcel::ConstFaceIterator fit = d.faceBegin(); fit != d.faceEnd(); ++fit){
        const Dcel::Face* of = *fit;
        Dcel::Face* f = this->addFace(of->getId());
        f->setId(of->getId());
        f->setColor(of->getColor());
        f->setFlag(of->getFlag());
        f->setNormal(of->getNormal());
        f->setArea(of->getArea());
        f->setOuterHalfEdge(mapHalfEdges[of->getOuterHalfEdge()]);
        for (Dcel::Face::ConstInnerHalfEdgeIterator heit = of->innerHalfEdgeBegin(); heit != of->innerHalfEdgeEnd(); ++heit){
            f->addInnerHalfEdge(mapHalfEdges[*heit]);
        }
        mapFaces[of] = f;
    }

    for (Dcel::ConstHalfEdgeIterator heit = d.halfEdgeBegin(); heit != d.halfEdgeEnd(); ++heit) {
        const Dcel::HalfEdge* ohe = *heit;
        Dcel::HalfEdge* he = mapHalfEdges[ohe];
        he->setNext(mapHalfEdges[ohe->getNext()]);
        he->setPrev(mapHalfEdges[ohe->getPrev()]);
        he->setTwin(mapHalfEdges[ohe->getTwin()]);
        he->setFace(mapFaces[ohe->getFace()]);
    }

    for (Dcel::ConstVertexIterator vit = d.vertexBegin(); vit != d.vertexEnd(); ++vit) {
        Dcel::Vertex * v = mapVertices[*vit];
        v->setIncidentHalfEdge(mapHalfEdges[(*vit)->getIncidentHalfEdge()]);
    }
}

#ifdef EIGENMESH_DEFINED
void Dcel::copyFrom(const SimpleEigenMesh& eigenMesh) {
    clear();

    std::vector<Vertex*> vertices;

    std::map< std::pair<int,int>, HalfEdge* > edge;
    std::map< std::pair<int,int>, HalfEdge* >::iterator eiter;

    bool first = true;


    for (unsigned int i = 0; i < eigenMesh.getNumberVertices(); i++) {

        Pointd coord = eigenMesh.getVertex(i);

        if (first) {
            boundingBox.setMin(coord);
            boundingBox.setMax(coord);
            first = false;
        }

        if (coord.x() < boundingBox.getMinX()) boundingBox.setMinX(coord.x());
        if (coord.y() < boundingBox.getMinY()) boundingBox.setMinY(coord.y());
        if (coord.z() < boundingBox.getMinZ()) boundingBox.setMinZ(coord.z());

        if (coord.x() > boundingBox.getMaxX()) boundingBox.setMaxX(coord.x());
        if (coord.y() > boundingBox.getMaxY()) boundingBox.setMaxY(coord.y());
        if (coord.z() > boundingBox.getMaxZ()) boundingBox.setMaxZ(coord.z());

        Vertex* vid = addVertex(coord);

        vertices.push_back(vid);
    }

    for (unsigned int i = 0; i < eigenMesh.getNumberFaces(); i++) {

        std::vector<int> nid;
        Pointi ff = eigenMesh.getFace(i);
        nid.push_back(ff.x());
        nid.push_back(ff.y());
        nid.push_back(ff.z());
        nid.push_back(nid[0]);
        std::pair<int, int> p;
        HalfEdge* eid = nullptr;
        Face* fid = addFace();
        HalfEdge* prev = nullptr;
        HalfEdge* first = nullptr;

        for (unsigned int i=0; i<nid.size()-1; i++){
            eid = addHalfEdge();
            if (i==0) {
                first = eid;
                fid->setOuterHalfEdge(eid);
            }
            else {
                eid->setPrev(prev);
                prev->setNext(eid);
            }
            vertices[nid[i]]->setIncidentHalfEdge(eid);
            eid->setFromVertex(vertices[nid[i]]);
            vertices[nid[i]]->incrementCardinality();
            eid->setToVertex(vertices[nid[i+1]]);
            eid->setFace(fid);
            p.first = nid[i+1];
            p.second = nid[i];
            eiter = edge.find(p);
            if (eiter != edge.end()){
                HalfEdge* twin = edge[p];
                eid->setTwin(twin);
                twin->setTwin(eid);
                edge.erase(eiter);
            }
            else {
                p.first = nid[i];
                p.second = nid[i+1];
                edge[p] = eid;
            }
            prev = eid;
        }
        eid->setNext(first);
        first->setPrev(eid);

        fid->setColor(Color(128, 128, 128));

        fid->updateNormal();
        fid->updateArea();
    }
}

void Dcel::copyFrom(const EigenMesh& eigenMesh) {
    copyFrom((SimpleEigenMesh)eigenMesh);
    for (Dcel::Face* f : faceIterator()){
        f->setColor(eigenMesh.getColor(f->getId()));
        f->setNormal(eigenMesh.getFaceNormal(f->getId()));
    }
    for (Dcel::Vertex* v : vertexIterator()){
        v->setNormal(eigenMesh.getVertexNormal(v->getId()));
    }
}
#endif

