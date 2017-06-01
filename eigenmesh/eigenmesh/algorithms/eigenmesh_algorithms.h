#ifndef EIGENMESHALGORITHMS_H
#define EIGENMESHALGORITHMS_H

#include <eigenmesh/eigenmesh.h>

class EigenMeshAlgorithms {
    public:
        static SimpleEigenMesh makeBox(const BoundingBox &bb, double minimumEdge = -1);
        static SimpleEigenMesh makeBox(const Pointd &min, const Pointd &max, double minimumEdge = -1);
        #ifdef IGL_DEFINED
        static SimpleEigenMesh decimateMesh(const SimpleEigenMesh& m, int numberDesiredFaces, Eigen::VectorXi &mapping = dummyEigenVectorXi);
        static EigenMesh decimateMesh(const EigenMesh& m, int numberDesiredFaces, Eigen::VectorXi &mapping = dummyEigenVectorXi);

        static void decimateMesh(SimpleEigenMesh& m, int numberDesiredFaces, Eigen::VectorXi &mapping = dummyEigenVectorXi);
        static void decimateMesh(EigenMesh& m, int numberDesiredFaces, Eigen::VectorXi &mapping = dummyEigenVectorXi);

        #ifdef CGAL_DEFINED
        static void intersection(SimpleEigenMesh &result, const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);
        static void intersection(EigenMesh &result, const EigenMesh &m1, const EigenMesh &m2);
        static SimpleEigenMesh intersection(const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);
        static EigenMesh intersection(const EigenMesh &m1, const EigenMesh &m2);

        static void difference(SimpleEigenMesh &result, const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);
        static void difference(EigenMesh &result, const EigenMesh &m1, const EigenMesh &m2);
        static SimpleEigenMesh difference(const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);
        static EigenMesh difference(const EigenMesh &m1, const EigenMesh &m2);

        static void union_(SimpleEigenMesh &result, const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);
        static void union_(EigenMesh &result, const EigenMesh &m1, const EigenMesh &m2);
        static SimpleEigenMesh union_(const SimpleEigenMesh &m1, const SimpleEigenMesh &m2);
        static EigenMesh union_(const EigenMesh &m1, const EigenMesh &m2);
        #endif

        static void removeUnreferencedVertices(SimpleEigenMesh &input, Eigen::Matrix<int, Eigen::Dynamic, 1> &I);
        static std::vector<SimpleEigenMesh> getConnectedComponents(const SimpleEigenMesh &m);
        static Eigen::MatrixXi getFaceAdjacences(const SimpleEigenMesh &m);

        #endif

     private:
          static Eigen::VectorXi dummyEigenVectorXi;
};

inline SimpleEigenMesh EigenMeshAlgorithms::makeBox(const Pointd &min, const Pointd &max, double minimumEdge){
    return makeBox(BoundingBox(min, max), minimumEdge);
}

#endif // EIGENMESHALGORITHMS_H
