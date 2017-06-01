#ifndef EIGENMESH_H
#define EIGENMESH_H

#include <Eigen/Core>

#include <common/serialize.h>
#include <common/point.h>
#include <common/bounding_box.h>
#include <common/color.h>

#ifdef DCEL_DEFINED
class Dcel;
#endif

#ifdef TRIMESH_DEFINED
template<typename T>
class Trimesh;
#endif

class EigenMeshAlgorithms;

class SimpleEigenMesh : public SerializableObject {
        friend class EigenMeshAlgorithms;

    public:
        SimpleEigenMesh();
        template <typename T, typename U, int ...A> SimpleEigenMesh(const Eigen::Matrix<T, A...> &V, const Eigen::Matrix<U, A...> &F);
        #ifdef DCEL_DEFINED
        SimpleEigenMesh(const Dcel& dcel);
        #endif
        #ifdef TRIMESH_DEFINED
        template<typename T> SimpleEigenMesh(const Trimesh<T>& trimesh);
        #endif

        const Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>& getVerticesMatrix() const;
        const Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>& getFacesMatrix() const;

        unsigned int getNumberVertices() const;
        unsigned int getNumberFaces() const;
        Pointd getVertex(unsigned int i) const;
        Pointi getFace(unsigned int i) const;
        virtual void getBoundingBox(Eigen::RowVector3d &BBmin, Eigen::RowVector3d &BBmax) const;
        virtual BoundingBox getBoundingBox() const;
        Pointd getBarycenter() const;

        virtual void clear();
        virtual void resizeVertices(unsigned int nv);
        void setVertex(unsigned int i, const Eigen::VectorXd &p);
        void setVertex(unsigned int i, const Pointd &p);
        void setVertex(unsigned int i, double x, double y, double z);
        virtual void addVertex(const Eigen::VectorXd &p);
        virtual void addVertex(const Pointd &p);
        virtual void addVertex(double x, double y, double z);
        virtual void resizeFaces(unsigned int nf);
        void setFace(unsigned int i, const Eigen::VectorXi &f);
        void setFace(unsigned int i, int t1, int t2, int t3);
        virtual void addFace(const Eigen::VectorXi &f);
        virtual void addFace(int t1, int t2, int t3);
        template <typename T, int ...A> void setVerticesMatrix(const Eigen::Matrix<T, A...>& V);
        template <typename U, int ...A> void setFacesMatrix(const Eigen::Matrix<U, A...>& F);


        virtual bool readFromObj(const std::string &filename);

        virtual bool saveOnPly(const std::string &filename) const;
        virtual bool saveOnObj(const std::string &filename) const;

        void translate(const Pointd &p);
        void translate(const Eigen::Vector3d &p);
        void rotate(const Eigen::Matrix3d &m, const Eigen::Vector3d& centroid = Eigen::Vector3d::Zero());
        void scale(const BoundingBox& newBoundingBox);
        void scale(const BoundingBox& oldBoundingBox, const BoundingBox& newBoundingBox);
        void scale(const Vec3 &scaleFactor);
        static void merge(SimpleEigenMesh &result, const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);
        static SimpleEigenMesh merge(const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);

        // SerializableObject interface
        virtual void serialize(std::ofstream& binaryFile) const;
        virtual bool deserialize(std::ifstream& binaryFile);


    protected:
        Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> V;
        Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> F;
        static Eigen::Matrix<int, Eigen::Dynamic, 1> dummy;
};

class EigenMesh : public SimpleEigenMesh {
        friend class EigenMeshAlgorithms;
    public:
        EigenMesh();
        EigenMesh(const SimpleEigenMesh &m);
        EigenMesh(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F);
        EigenMesh(const Eigen::MatrixXd &V, const Eigen::MatrixXi &F, const Eigen::MatrixXf& CV, const Eigen::MatrixXf& CF);
        #ifdef DCEL_DEFINED
        EigenMesh(const Dcel& dcel);
        #endif
        #ifdef TRIMESH_DEFINED
        template<typename T>
        EigenMesh(const Trimesh<T>& trimesh);
        #endif
        virtual void resizeVertices(unsigned int nv);
        virtual void resizeFaces(unsigned int nf);
        void updateBoundingBox();
        virtual void clear();
        virtual bool readFromObj(const std::string &filename);
        void setFaceColor(double red, double green, double blue, int f = -1);
        Vec3 getFaceNormal(unsigned int f) const;
        Vec3 getVertexNormal(unsigned int v) const;
        Color getColor(unsigned int f) const;
        virtual void getBoundingBox(Eigen::RowVector3d &BBmin, Eigen::RowVector3d &BBmax) const;
        virtual BoundingBox getBoundingBox() const;
        void scale(const BoundingBox& newBoundingBox);
        void scale(const BoundingBox& oldBoundingBox, const BoundingBox& newBoundingBox);
        void scale(const Vec3 &scaleFactor);
        Eigen::MatrixXf getVerticesColorMatrix() const;
        void updateFaceNormals();
        void updateVerticesNormals();
        void updateFacesAndVerticesNormals();

        virtual bool saveOnPly(const std::string &filename) const;
        virtual bool saveOnObj(const std::string &filename) const;

        static void merge(EigenMesh &result, const EigenMesh &m1, const EigenMesh &m2);
        static EigenMesh merge(const EigenMesh &m1, const EigenMesh &m2);

        #ifdef DCEL_DEFINED
        EigenMesh& operator= (const Dcel& dcel);
        #endif

        void serialize(std::ofstream& binaryFile) const;
        bool deserialize(std::ifstream& binaryFile);

    protected:

        void updateFaceColorsSize();
        void updateVertexColorsSize();
        void updateColorSizes();

        //Eigen::RowVector3d BBmin, BBmax;
        BoundingBox bb;
        Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> NV;
        Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> NF;
        Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> CV;
        Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::RowMajor> CF;
};

/**
 * SimpleEigenMesh
 */

inline SimpleEigenMesh::SimpleEigenMesh() {
}

template <typename T, typename U, int ...A>
SimpleEigenMesh::SimpleEigenMesh(const Eigen::Matrix<T, A...> &V, const Eigen::Matrix<U, A...> &F) : V(V), F(F) {
}

#ifdef TRIMESH_DEFINED
template <typename T>
inline SimpleEigenMesh::SimpleEigenMesh(const Trimesh<T>& trimesh) {
    int numV=trimesh.numVertices();
    int numF=trimesh.numTriangles();

    clear();
    V.resize(numV,3);
    F.resize(numF,3);

    for(int i=0;i<numV;++i) {
        V(i,0)=trimesh.vertex(i).x();
        V(i,1)=trimesh.vertex(i).y();
        V(i,2)=trimesh.vertex(i).z();
    }

    for(int i=0;i<numF;++i) {
        F(i,0)=trimesh.tri_vertex_id(i,0);
        F(i,1)=trimesh.tri_vertex_id(i,1);
        F(i,2)=trimesh.tri_vertex_id(i,2);
    }
}
#endif

inline const Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor>& SimpleEigenMesh::getVerticesMatrix() const {
    return V;
}

inline const Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor>& SimpleEigenMesh::getFacesMatrix() const {
    return F;
}

inline unsigned int SimpleEigenMesh::getNumberVertices() const {
    return V.rows();
}

inline unsigned int SimpleEigenMesh::getNumberFaces() const {
    return F.rows();
}

inline Pointd SimpleEigenMesh::getVertex(unsigned int i) const {
    assert(i < V.rows());
    return Pointd(V(i,0), V(i,1), V(i,2));
}

inline Pointi SimpleEigenMesh::getFace(unsigned int i) const {
    assert (i < F.rows());
    return Pointi(F(i,0), F(i,1), F(i,2));
}

inline void SimpleEigenMesh::getBoundingBox(Eigen::RowVector3d& BBmin, Eigen::RowVector3d& BBmax) const {
    if (V.rows() > 0){
        BBmin = V.colwise().minCoeff();
        BBmax = V.colwise().maxCoeff();
    }
    else {
        BBmin = Eigen::RowVector3d();
        BBmax = Eigen::RowVector3d();
    }
}

inline BoundingBox SimpleEigenMesh::getBoundingBox() const {
    BoundingBox  bb;
    if (V.rows() > 0){
        Eigen::RowVector3d BBmin, BBmax;
        BBmin = V.colwise().minCoeff();
        BBmax = V.colwise().maxCoeff();
        bb.setMin(BBmin(0), BBmin(1), BBmin(2));
        bb.setMax(BBmax(0), BBmax(1), BBmax(2));
    }
    return bb;
}

inline Pointd SimpleEigenMesh::getBarycenter() const {
    Pointd bc(V.col(0).mean(), V.col(1).mean(), V.col(2).mean());
    return bc;
}

inline void SimpleEigenMesh::clear() {
    V.resize(0,Eigen::NoChange);
    F.resize(0,Eigen::NoChange);
}

inline void SimpleEigenMesh::resizeVertices(unsigned int nv) {
    V.conservativeResize(nv,Eigen::NoChange);
}

inline void SimpleEigenMesh::setVertex(unsigned int i, const Eigen::VectorXd& p) {
    assert (i < V.rows());
    assert (p.size() == 3);
    V.row(i) =  p;
}

inline void SimpleEigenMesh::setVertex(unsigned int i, const Pointd& p) {
    assert (i < V.rows());
    V(i,0) = p[0]; V(i,1) = p[1]; V(i,2) = p[2];
}

inline void SimpleEigenMesh::setVertex(unsigned int i, double x, double y, double z) {
    assert (i < V.rows());
    V(i, 0) = x; V(i, 1) = y; V(i, 2) = z;
}

inline void SimpleEigenMesh::addVertex(const Eigen::VectorXd& p) {
    assert (p.size() == 3);
    V.conservativeResize(V.rows()+1, Eigen::NoChange);
    V.row(V.rows()-1) = p;
}

inline void SimpleEigenMesh::addVertex(const Pointd& p) {
    V.conservativeResize(V.rows()+1, Eigen::NoChange);
    V(V.rows()-1, 0) = p.x(); V(V.rows()-1, 1) = p.y(); V(V.rows()-1, 2) = p.z();
}

inline void SimpleEigenMesh::addVertex(double x, double y, double z) {
    V.conservativeResize(V.rows()+1, Eigen::NoChange);
    V(V.rows()-1, 0) = x; V(V.rows()-1, 1) = y; V(V.rows()-1, 2) = z;
}

inline void SimpleEigenMesh::resizeFaces(unsigned int nf) {
    F.conservativeResize(nf,Eigen::NoChange);
}

inline void SimpleEigenMesh::setFace(unsigned int i, const Eigen::VectorXi& f) {
    assert (i < F.rows());
    assert (f.size() == 3);
    F.row(i) =  f;
}

inline void SimpleEigenMesh::setFace(unsigned int i, int t1, int t2, int t3) {
    assert (i < F.rows());
    F(i, 0) = t1; F(i, 1) = t2; F(i, 2) = t3;
}

inline void SimpleEigenMesh::addFace(const Eigen::VectorXi& f) {
    assert (f.size() == 3);
    F.conservativeResize(F.rows()+1, Eigen::NoChange);
    F.row(F.rows()-1) = f;
}

inline void SimpleEigenMesh::addFace(int t1, int t2, int t3) {
    F.conservativeResize(F.rows()+1, Eigen::NoChange);
    F(F.rows()-1, 0) = t1; F(F.rows()-1, 1) = t2; F(F.rows()-1, 2) = t3;
}


template <typename T, int ...A>
inline void SimpleEigenMesh::setVerticesMatrix(const Eigen::Matrix<T, A...>& V) {
    this->V = V;
}

template <typename U, int ...A>
inline void SimpleEigenMesh::setFacesMatrix(const Eigen::Matrix<U, A...>& F) {
    this->F = F;
}

inline void SimpleEigenMesh::serialize(std::ofstream& binaryFile) const {
    Serializer::serialize(V, binaryFile);
    Serializer::serialize(F, binaryFile);
}

inline bool SimpleEigenMesh::deserialize(std::ifstream& binaryFile) {
    SimpleEigenMesh tmp;
    if (Serializer::deserialize(tmp.V, binaryFile) &&
            Serializer::deserialize(tmp.F, binaryFile)){
        *this = std::move(tmp);
        return true;
    }
    return false;
}

/**
  * EigenMesh
  */

#ifdef TRIMESH_DEFINED
template <typename T>
inline EigenMesh::EigenMesh(const Trimesh<T>& trimesh) : SimpleEigenMesh(trimesh)
{
    CF = Eigen::MatrixXd::Constant(F.rows(), 3, 0.5);
    NV.resize(V.rows(), 3);
    NF.resize(F.rows(), 3);
    updateFaceNormals();
    updateVerticesNormals();
    updateBoundingBox();
}
#endif

inline void EigenMesh::resizeVertices(unsigned int nv){
    SimpleEigenMesh::resizeVertices(nv);
    NV.conservativeResize(nv, Eigen::NoChange);
}

inline void EigenMesh::resizeFaces(unsigned int nf) {
    SimpleEigenMesh::resizeFaces(nf);
    NF.conservativeResize(nf, Eigen::NoChange);
    CF.conservativeResize(nf, Eigen::NoChange);
}

inline void EigenMesh::updateBoundingBox() {
    if (V.rows() > 0){
        Eigen::RowVector3d min = V.colwise().minCoeff(), max = V.colwise().maxCoeff();
        bb.min().x() = min(0); bb.min().y() = min(1); bb.min().z() = min(2);
        bb.max().x() = max(0); bb.max().y() = max(1); bb.max().z() = max(2);
    }
}

inline void EigenMesh::clear() {
    V.resize(0,Eigen::NoChange);
    F.resize(0,Eigen::NoChange);
    CF.resize(0,Eigen::NoChange);
    NV.resize(0,Eigen::NoChange);
    NF.resize(0,Eigen::NoChange);
}

inline Vec3 EigenMesh::getFaceNormal(unsigned int f) const {
    assert (f < F.rows());
    return Vec3(NF(f,0), NF(f,1), NF(f,2));
}

inline Vec3 EigenMesh::getVertexNormal(unsigned int v) const {
    assert (v < V.rows());
    return Vec3(NV(v,0), NV(v,1), NV(v,2));
}

inline Color EigenMesh::getColor(unsigned int f) const {
    assert (f < F.rows());
    Color c;
    c.setRedF((float)CF(f,0));
    c.setGreenF((float)CF(f,1));
    c.setBlueF((float)CF(f,2));
    return c;
}

inline void EigenMesh::getBoundingBox(Eigen::RowVector3d& BBmin, Eigen::RowVector3d& BBmax) const {
    BBmin(0) = bb.minX(); BBmin(1) = bb.minY(); BBmin(2) = bb.minZ();
    BBmax(0) = bb.maxX(); BBmax(1) = bb.maxY(); BBmax(2) = bb.maxZ();
}

inline void EigenMesh::updateFacesAndVerticesNormals() {
    updateFaceNormals();
    updateVerticesNormals();
}

#endif // EIGENMESH_H
