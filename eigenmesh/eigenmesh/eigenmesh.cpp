#include "eigenmesh.h"
#include <boost/tokenizer.hpp>
#include "common/timer.h"
#include "common/load_save_file.h"

#ifdef DCEL_DEFINED
#include "dcel/dcel.h"
#endif

#ifdef TRIMESH_DEFINED
#include "trimesh/trimesh.h"
#endif



#ifdef DCEL_DEFINED
SimpleEigenMesh::SimpleEigenMesh(const Dcel& dcel) {
    clear();
    V.resize(dcel.getNumberVertices(), 3);
    F.resize(dcel.getNumberFaces(), 3);
    std::map<int, int> vids;
    unsigned int i = 0;
    for (Dcel::ConstVertexIterator vit = dcel.vertexBegin(); vit != dcel.vertexEnd(); ++vit){
        const Dcel::Vertex* v = *vit;
        vids[v->getId()] = i;
        Pointd p = v->getCoordinate();
        V(i,0) = p.x(); V(i,1) = p.y(); V(i,2) = p.z();
        i++;
    }
    i = 0;
    for (Dcel::ConstFaceIterator fit = dcel.faceBegin(); fit != dcel.faceEnd(); ++fit){
        const Dcel::Face* f = *fit;
        F(i, 0) = vids[f->getVertex1()->getId()]; F(i, 1) = vids[f->getVertex2()->getId()]; F(i, 2) = vids[f->getVertex3()->getId()];
        i++;
    }
}
#endif

bool SimpleEigenMesh::readFromObj(const std::string& filename) {
    return Common::loadTriangleMeshFromObj(filename, V, F);
}

bool SimpleEigenMesh::saveOnPly(const std::string& filename) const {
    return Common::saveMeshOnPly(filename, V.rows(), F.rows(), V.data(), F.data());
}

bool SimpleEigenMesh::saveOnObj(const std::string& filename) const {
    return Common::saveMeshOnObj(filename, V.rows(), F.rows(), V.data(), F.data());
}

void SimpleEigenMesh::translate(const Pointd& p) {
    Eigen::RowVector3d v;
    v << p.x(), p.y(), p.z();
    V.rowwise() += v;
}

void SimpleEigenMesh::translate(const Eigen::Vector3d& p) {
    V.rowwise() += p.transpose();
}

void SimpleEigenMesh::rotate(const Eigen::Matrix3d& m, const Eigen::Vector3d& centroid) {
    V.rowwise() -= centroid.transpose();
    for (unsigned int i = 0; i < V.rows(); i++){
        V.row(i) =  m * V.row(i).transpose();
    }
    V.rowwise() += centroid.transpose();
}

void SimpleEigenMesh::scale(const BoundingBox& newBoundingBox) {
    BoundingBox boundingBox = getBoundingBox();
    Pointd oldCenter = boundingBox.center();
    Pointd newCenter = newBoundingBox.center();
    Pointd deltaOld = boundingBox.getMax() - boundingBox.getMin();
    Pointd deltaNew = newBoundingBox.getMax() - newBoundingBox.getMin();
    for (int i = 0; i < V.rows(); i++){
        Pointd coord = getVertex(i);
        coord -= oldCenter;
        coord *= deltaNew / deltaOld;
        coord += newCenter;
        setVertex(i, coord);
    }
}

void SimpleEigenMesh::scale(const BoundingBox& oldBoundingBox, const BoundingBox& newBoundingBox) {
    Pointd oldCenter = oldBoundingBox.center();
    Pointd newCenter = newBoundingBox.center();
    Pointd deltaOld = oldBoundingBox.getMax() - oldBoundingBox.getMin();
    Pointd deltaNew = newBoundingBox.getMax() - newBoundingBox.getMin();
    for (int i = 0; i < V.rows(); i++){
        Pointd coord = getVertex(i);
        coord -= oldCenter;
        coord *= deltaNew / deltaOld;
        coord += newCenter;
        setVertex(i, coord);
    }
}

void SimpleEigenMesh::scale(const Vec3& scaleFactor) {
    if (scaleFactor.x() > 0 && scaleFactor.y() > 0 && scaleFactor.z() > 0){
        BoundingBox bb = getBoundingBox();
        Pointd center = bb.center();
        Pointd newMax(bb.min().x() + bb.getLengthX()*scaleFactor.x(), bb.min().y() + bb.getLengthY()*scaleFactor.y(), bb.min().z() + bb.getLengthZ()*scaleFactor.z());
        bb.setMax(newMax);
        Pointd trans = center - bb.center();
        bb.min() += trans;
        bb.max() += trans;
        scale(bb);
    }
}

void SimpleEigenMesh::merge(SimpleEigenMesh &result, const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    result.V.resize(m1.V.rows()+m2.V.rows(), 3);
    result.V << m1.V,
            m2.V;
    result.F = m1.F;
    int start = m1.getNumberVertices();
    for (unsigned int i = 0; i < m2.getNumberFaces(); i++){
        Pointi fi =m2.getFace(i);
        result.addFace(fi.x()+start, fi.y()+start, fi.z()+start);
    }
}

SimpleEigenMesh SimpleEigenMesh::merge(const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    SimpleEigenMesh result;
    result.V.resize(m1.V.rows()+m2.V.rows(), 3);
    result.V << m1.V,
            m2.V;
    result.F = m1.F;
    int start = m1.getNumberVertices();
    for (unsigned int i = 0; i < m2.getNumberFaces(); i++){
        Pointi fi =m2.getFace(i);
        result.addFace(fi.x()+start, fi.y()+start, fi.z()+start);
    }
    return result;
}


EigenMesh::EigenMesh() {
}

EigenMesh::EigenMesh(const SimpleEigenMesh& m) : SimpleEigenMesh(m) {
    updateColorSizes();
    updateFaceNormals();
    updateVerticesNormals();
    updateBoundingBox();
}

EigenMesh::EigenMesh(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F) : SimpleEigenMesh(V,F) {
    updateColorSizes();
    updateFaceNormals();
    updateVerticesNormals();
    updateBoundingBox();
}

EigenMesh::EigenMesh(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F, const Eigen::MatrixXf& CV, const Eigen::MatrixXf& CF) : SimpleEigenMesh(V,F), CV(CV), CF(CF) {
    updateFaceNormals();
    updateVerticesNormals();
    updateBoundingBox();
}

#ifdef DCEL_DEFINED
EigenMesh::EigenMesh(const Dcel& dcel) {
    clear();
    V.resize(dcel.getNumberVertices(), 3);
    F.resize(dcel.getNumberFaces(), 3);
    CF.resize(F.rows(), 3);
    updateVertexColorsSize();
    NV.resize(V.rows(), 3);
    NF.resize(F.rows(), 3);
    std::map<int, int> vids;
    bb = dcel.getBoundingBox();
    unsigned int i = 0;
    for (Dcel::ConstVertexIterator vit = dcel.vertexBegin(); vit != dcel.vertexEnd(); ++vit){
        const Dcel::Vertex* v = *vit;
        vids[v->getId()] = i;
        Pointd p = v->getCoordinate();
        Vec3 n = v->getNormal();
        V(i,0) = p.x(); V(i,1) = p.y(); V(i,2) = p.z();
        NV(i,0) = n.x(); NV(i,1) = n.y(); NV(i,2) = n.z();
        i++;
    }
    i = 0;
    for (Dcel::ConstFaceIterator fit = dcel.faceBegin(); fit != dcel.faceEnd(); ++fit){
        const Dcel::Face* f = *fit;
        F(i, 0) = vids[f->getVertex1()->getId()]; F(i, 1) = vids[f->getVertex2()->getId()]; F(i, 2) = vids[f->getVertex3()->getId()];
        Color c = f->getColor();
        Vec3 n = f->getNormal();
        CF(i,0) = c.redF(); CF(i,1) = c.greenF(); CF(i,2) = c.blueF();
        NF(i,0) = n.x(); NF(i,1) = n.y(); NF(i,2) = n.z();
        i++;
    }
}

#endif

bool EigenMesh::readFromObj(const std::string& filename) {
    clear();
    int mode = 0;
    bool b = Common::loadTriangleMeshFromObj(filename, V, F, mode, NV, CV, CF);
    updateBoundingBox();

    if (b){
        updateFaceNormals();
        if (!(mode & Common::NORMAL_VERTICES)){
            updateVerticesNormals();
        }
        if (!(mode & Common::COLOR_VERTICES)){
            updateVertexColorsSize();
        }
        if (!(mode & Common::COLOR_FACES)){
            updateFaceColorsSize();
        }
    }
    return b;
}

void EigenMesh::setFaceColor(double red, double green, double blue, int f) {
    if (f < 0){
        CF.resize(F.rows(), 3);
        for (unsigned int i = 0; i < CF.rows(); i++)
            CF.row(i) << red, green, blue;
    }
    else{
        assert(f < F.rows());
        CF.row(f) << red, green, blue;
    }
}

BoundingBox EigenMesh::getBoundingBox() const {
    return bb;
}

void EigenMesh::scale(const BoundingBox& newBoundingBox) {
    SimpleEigenMesh::scale(newBoundingBox);
    bb = newBoundingBox;
}

void EigenMesh::scale(const BoundingBox& oldBoundingBox, const BoundingBox& newBoundingBox) {
    SimpleEigenMesh::scale(oldBoundingBox, newBoundingBox);
    bb = newBoundingBox;
}

void EigenMesh::scale(const Vec3& scaleFactor) {
    SimpleEigenMesh::scale(scaleFactor);
    updateBoundingBox();
}

Eigen::MatrixXf EigenMesh::getVerticesColorMatrix() const {
    return CV;
}

void EigenMesh::updateFaceNormals() {
    Eigen::Matrix<double,3,1> Z(0,0,0);
    NF.resize(F.rows(),3);
    int nf = F.rows();
    //#pragma omp parallel for
    for(int i = 0; i < nf; i++) {
        const Eigen::Matrix<double, 1, 3, Eigen::RowMajor> v1 = V.row(F(i,1)) - V.row(F(i,0));
        const Eigen::Matrix<double, 1, 3, Eigen::RowMajor> v2 = V.row(F(i,2)) - V.row(F(i,0));
        Pointd p1(v1(0,0),v1(0,1),v1(0,2));
        Pointd p = p1.cross(Pointd(v2(0,0),v2(0,1),v2(0,2)));
        //NF.row(i) = v1.cross(v2);//.normalized(); //eigen probelm
        NF(i,0) = p.x(); NF(i,1) = p.y(); NF(i,2) = p.z();
        double r = NF.row(i).norm();
        if(r == 0) {
            NF.row(i) = Z;
        }
        else {
            NF.row(i) /= r;
        }
    }
}

void EigenMesh::updateVerticesNormals() {
    NV = Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>::Zero(V.rows(), 3);
    // loop over faces
    for(int i = 0;i<F.rows();i++) {
        // throw normal at each corner
        for(int j = 0; j < 3;j++) {
            NV.row(F(i,j)) += NF.row(i);
        }
    }
    NV.rowwise().normalize();
}

bool EigenMesh::saveOnPly(const std::string &filename) const {
    int mode = Common::NORMAL_VERTICES | Common::COLOR_VERTICES | Common::COLOR_FACES;
    Common::MeshType meshType;
    if (F.cols() == 3)
        meshType = Common::TRIANGLE_MESH;
    else if (F.cols() == 4)
        meshType = Common::QUAD_MESH;
    else
        meshType = Common::POLYGON_MESH;
    Common::ColorMode colorMode = Common::RGB;
    return Common::saveMeshOnPly(filename, V.rows(), F.rows(), V.data(), F.data(), meshType, mode, NV.data(), colorMode, CV.data(), CF.data());
}

bool EigenMesh::saveOnObj(const std::string& filename) const {
    int mode = Common::TRIANGLE_MESH | Common::NORMAL_VERTICES | Common::COLOR_VERTICES | Common::COLOR_FACES;
    Common::MeshType meshType;
       if (F.cols() == 3)
           meshType = Common::TRIANGLE_MESH;
       else if (F.cols() == 4)
           meshType = Common::QUAD_MESH;
       else
           meshType = Common::POLYGON_MESH;
    Common::ColorMode colorMode = Common::RGB;
    return Common::saveMeshOnObj(filename, V.rows(), F.rows(), V.data(), F.data(), meshType, mode, NV.data(), colorMode, CV.data(), CF.data());
}

void EigenMesh::merge(EigenMesh& result, const EigenMesh& m1, const EigenMesh& m2) {
    SimpleEigenMesh::merge(result, m1, m2);
    result.CF.resize(m1.CF.rows()+m2.CF.rows(), 3);
    result.CF << m1.CF,
            m2.CF;
    result.CV.resize(m1.CV.rows()+m2.CV.rows(), 3);
    result.CV << m1.CV,
            m2.CV;
    result.NV.resize(m1.NV.rows()+m2.NV.rows(), 3);
    result.NV << m1.NV,
            m2.NV;
    result.NF.resize(m1.NF.rows()+m2.NF.rows(), 3);
    result.NF << m1.NF,
            m2.NF;
    result.updateBoundingBox();
}

EigenMesh EigenMesh::merge(const EigenMesh& m1, const EigenMesh& m2) {
    EigenMesh result;
    SimpleEigenMesh::merge(result, m1, m2);
    result.CF.resize(m1.CF.rows()+m2.CF.rows(), 3);
    result.CF << m1.CF,
            m2.CF;
    result.CV.resize(m1.CV.rows()+m2.CV.rows(), 3);
    result.CV << m1.CV,
            m2.CV;
    result.NV.resize(m1.NV.rows()+m2.NV.rows(), 3);
    result.NV << m1.NV,
            m2.NV;
    result.NF.resize(m1.NF.rows()+m2.NF.rows(), 3);
    result.NF << m1.NF,
            m2.NF;
    result.updateBoundingBox();
    return result;
}

#ifdef DCEL_DEFINED
EigenMesh& EigenMesh::operator=(const Dcel& dcel) {
    clear();
    V.resize(dcel.getNumberVertices(), 3);
    F.resize(dcel.getNumberFaces(), 3);
    CF.resize(F.rows(), 3);
    CV = Eigen::MatrixXf::Constant(V.rows(), 3, 0.5);
    NV.resize(V.rows(), 3);
    NF.resize(F.rows(), 3);
    std::map<int, int> vids;
    bb = dcel.getBoundingBox();
    unsigned int i = 0;
    for (Dcel::ConstVertexIterator vit = dcel.vertexBegin(); vit != dcel.vertexEnd(); ++vit){
        const Dcel::Vertex* v = *vit;
        vids[v->getId()] = i;
        Pointd p = v->getCoordinate();
        Vec3 n = v->getNormal();
        V(i,0) = p.x(); V(i,1) = p.y(); V(i,2) = p.z();
        NV(i,0) = n.x(); NV(i,1) = n.y(); NV(i,2) = n.z();
        i++;
    }
    i = 0;
    for (Dcel::ConstFaceIterator fit = dcel.faceBegin(); fit != dcel.faceEnd(); ++fit){
        const Dcel::Face* f = *fit;
        F(i, 0) = vids[f->getVertex1()->getId()]; F(i, 1) = vids[f->getVertex2()->getId()]; F(i, 2) = vids[f->getVertex3()->getId()];
        Color c = f->getColor();
        Vec3 n = f->getNormal();
        CF(i,0) = c.redF(); CF(i,1) = c.greenF(); CF(i,2) = c.blueF();
        NF(i,0) = n.x(); NF(i,1) = n.y(); NF(i,2) = n.z();
        i++;
    }
    return *this;
}
#endif

void EigenMesh::serialize(std::ofstream& binaryFile) const {
    Serializer::serialize(V, binaryFile);
    Serializer::serialize(F, binaryFile);
    Serializer::serialize(NV, binaryFile);
    Serializer::serialize(NF, binaryFile);
    Serializer::serialize(CV, binaryFile);
    Serializer::serialize(CF, binaryFile);
}

bool EigenMesh::deserialize(std::ifstream& binaryFile) {
    EigenMesh tmp;
    if (Serializer::deserialize(tmp.V, binaryFile) &&
            Serializer::deserialize(tmp.F, binaryFile) &&
            Serializer::deserialize(tmp.NV, binaryFile) &&
            Serializer::deserialize(tmp.NF, binaryFile) &&
            Serializer::deserialize(tmp.CV, binaryFile) &&
            Serializer::deserialize(tmp.CF, binaryFile)){
        tmp.updateBoundingBox();
        *this = std::move(tmp);
        return true;
    }
    return false;
}

void EigenMesh::updateFaceColorsSize() {
    CF.conservativeResizeLike(Eigen::MatrixXf::Constant(F.rows(), 3, 0.5));
}

void EigenMesh::updateVertexColorsSize() {
    CV.conservativeResizeLike(Eigen::MatrixXf::Constant(V.rows(), 3, 0.5));
}

void EigenMesh::updateColorSizes() {
    updateFaceColorsSize();
    updateVertexColorsSize();
}
