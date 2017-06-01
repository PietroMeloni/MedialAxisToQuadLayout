#include "dcel_vertex.h"
#include "dcel_vertex_iterators.h"

/*************************
 * Public Inline Methods *
 *************************/

/**
 * \~Italian
 * @brief Restirìtuisce l'id identificativo nella Dcel del vertice
 * @return L'id del vertice
 */
inline unsigned int Dcel::Vertex::getId() const {
    return id;
}

/**
 * \~Italian
 * @brief Restituisce il flag associato al vertice
 * @return Il flag del vertice
 */
inline int Dcel::Vertex::getFlag() const {
    return flag;
}

/**
 * \~Italian
 * @brief Restituisce il vettore normale al vertice
 * @note Non ricalcola la normale, restituisce solo l'ultima normale calcolata o settata
 * @return La normale al vertice
 */
inline Vec3 Dcel::Vertex::getNormal() const	{
    return parent.vertexNormals[id];
    //return normal;
}

/**
 * \~Italian
 * @brief Restituisce le coordinate del vertice
 * @return Pointd rappresentante la posizione nello spazio del vertice
 */
inline Pointd Dcel::Vertex::getCoordinate() const {
    //return coordinate;
    return parent.vertexCoordinates[id];
}

/**
 * \~Italian
 * @brief Restituisce il colore del vertice
 * @return Color rappresentante la posizione nello spazio del vertice
 */
inline Color Dcel::Vertex::getColor() const {
    //return coordinate;
    return parent.vertexColors[id];
}

/**
 * \~Italian
 * @brief Restituisce il numero di edge incidenti sul vertice
 * @note Non ricalcola la cardinalità, restituisce solo l'ultima cardinalità calcolata o settata
 * @return La cardinalità del vertice
 */
inline int  Dcel::Vertex::getCardinality() const {
    return cardinality;
}

/**
 * \~Italian
 * @brief Restituisce il puntatore l'half edge costante incidente sul vertice
 * @return L'HalfEdge incidente sul vertice
 */
inline const Dcel::HalfEdge* Dcel::Vertex::getIncidentHalfEdge() const {
    return incidentHalfEdge;
}

/**
 * \~Italian
 * @brief Calcola e restituisce la distanza tra il vertice this e il vertice in input
 * @param[in] otherVertex: vertice con cui verrà calcolata la distanza dal vertice this
 * @return La distanza tra il vertice this e otherVertex
 */
inline double Dcel::Vertex::dist(const Vertex* otherVertex) const {
    //return this->coordinate.dist(otherVertex->coordinate);
    return parent.vertexCoordinates[id].dist(parent.vertexCoordinates[otherVertex->id]);
}

/**
 * \~Italian
 * @brief Operatore di uguaglianza tra vertici
 * @param[in] otherVertex: vertice con cui verrà verificata l'uguaglianza con la faccia this
 * @return True se i vertici sono uguali, false altrimenti
 * @todo Da riscrivere
 */
inline bool Dcel::Vertex::operator == (const Vertex& otherVertex) const {
    //if (otherVertex.coordinate == this->coordinate ) return true;
    if (parent.vertexCoordinates[id] == parent.vertexCoordinates[otherVertex.id]) return true;
    return false;
}

/**
 * \~Italian
 * @brief Operatore di disuguaglianza tra vertici
 * @param[in] otherVertex: vertice con cui verrà verificata la disuguaglianza con la faccia this
 * @return True se i vertici sono diversi, false altrimenti
 */
inline bool Dcel::Vertex::operator != (const Vertex& otherVertex) const {
    //if (otherVertex.coordinate == this->coordinate ) return false;
    if (parent.vertexCoordinates[id] == parent.vertexCoordinates[otherVertex.id]) return false;
    return true;
}

/**
 * \~Italian
 * @brief Restituisce true se l'incident half edge è diverso da nullptr
 */
inline bool Dcel::Vertex::checkIncidentHalfEdge() const {
    return incidentHalfEdge != nullptr;
}

/**
 * \~Italian
 * @brief Setta il flag del vertice a 1
 */
inline void Dcel::Vertex::setFlag() {
    flag = 1;
}

/**
 * \~Italian
 *
 * @brief Setta il flag del vertice
 * @param[in] newFlag: il valore del flag che verrà settato
 */
inline void Dcel::Vertex::setFlag(int newFlag) {
    flag = newFlag;
}

/**
 * \~Italian
 * @brief Setta il flag della faccia a 0
 */
inline void Dcel::Vertex::resetFlag() {
    flag = 0;
}

/**
 * \~Italian
 * @brief Setta il vettore normale al vertice
 * @param[in] newNormal: il vettore normale che verrà settato
 */
inline void Dcel::Vertex::setNormal(const Vec3& newNormal) {
    parent.vertexNormals[id] = newNormal;
    //normal = newNormal;
}

/**
 * \~Italian
 * @brief Setta le coordinate del vertice
 * @param[in] newCoordinate: il punto che verrà settato
 */
inline void Dcel::Vertex::setCoordinate(const Pointd& newCoordinate) {
    parent.vertexCoordinates[id] = newCoordinate;
    //coordinate = newCoordinate;
}

/**
 * \~Italian
 * @brief Setta la cardinalità del vertice
 * @param[in] newCardinality: la cardinalitù che verrà settata
 */
inline void Dcel::Vertex::setCardinality( int newCardinality ) {
    cardinality = newCardinality;
}

inline void Dcel::Vertex::setColor(const Color& c) {
    parent.vertexColors[id] = c;
}

/**
 * \~Italian
 * @brief Decrementa la cardinalità del vertice (se essa è maggiore di 0)
 * @return La cardinalità decrementata
 */
inline int Dcel::Vertex::decrementCardinality() {
    if (cardinality > 0) --cardinality;
    return cardinality;
}

/**
 * \~Italian
 * @brief Incrementa la cardinalità del vertice
 * @return La cardinalità incrementata
 */
inline int Dcel::Vertex::incrementCardinality() {
    return ++cardinality;
}

/**
 * \~Italian
 * @brief Restituisce il puntatore all'half edge incidente sul vertice
 * @return L'HalfEdge incidente sul vertice
 */
inline Dcel::HalfEdge* Dcel::Vertex::getIncidentHalfEdge() {
    return incidentHalfEdge;
}

/**
 * \~Italian
 * @brief Assegna un nuovo half edge incidente al vertice
 * @param[in] newIncidentHalfEdge: puntatore all'half edge incidente assegnato al vertice
 */
inline void Dcel::Vertex::setIncidentHalfEdge(HalfEdge* newIncidentHalfEdge ) {
    incidentHalfEdge = newIncidentHalfEdge;
}

/****************************
 * Protected Inline Methods *
 ****************************/

/**
 * \~Italian
 * @brief Setta l'id del vertice.
 *
 * Questa funzione dovrebbe essere chiamata solamente dalla classe Dcel.
 *
 * @param[in] id: nuovo id che verrà assegnato al vertice
 */
inline void Dcel::Vertex::setId(unsigned int id)
{
    this->id = id;
}
