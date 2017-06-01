#include "eigenmesh_algorithms.h"

#ifdef IGL_DEFINED
#include <igl/decimate.h>
#include <igl/facet_components.h>
#include <igl/remove_unreferenced.h>
#include <igl/triangle_triangle_adjacency.h>


#ifdef CGAL_DEFINED
#include <igl/copyleft/cgal/CSGTree.h>
#endif
#endif

SimpleEigenMesh EigenMeshAlgorithms::makeBox(const BoundingBox& bb, double minimumEdge) {
    SimpleEigenMesh box;
    if (minimumEdge <= 0) {
        std::vector<Pointd> extremes;
        bb.getExtremes(extremes);
        box.resizeVertices(8);
        for (unsigned int i = 0; i < 8; i++){
            box.setVertex(i, extremes[i].x(), extremes[i].y(), extremes[i].z());
        }
        box.resizeFaces(12);
        box.setFace(0, 0,1,2);
        box.setFace(1, 0,2,3);
        box.setFace(2, 2,1,5);
        box.setFace(3, 2,5,6);
        box.setFace(4, 5,1,0);
        box.setFace(5, 5,0,4);
        box.setFace(6, 6,5,4);
        box.setFace(7, 6,4,7);
        box.setFace(8, 7,4,0);
        box.setFace(9, 7,0,3);
        box.setFace(10, 7,3,2);
        box.setFace(11, 7,2,6);
    }
    else {
        unsigned int nSubdX, nSubdY, nSubdZ;
        //minimumEdge /= std::sqrt(2.f);
        nSubdX = bb.getLengthX() / minimumEdge; nSubdX++;
        nSubdY = bb.getLengthY() / minimumEdge; nSubdY++;
        nSubdZ = bb.getLengthZ() / minimumEdge; nSubdZ++;
        double edgeLengthX = bb.getLengthX() / nSubdX;
        double edgeLengthY = bb.getLengthY() / nSubdY;
        double edgeLengthZ = bb.getLengthZ() / nSubdZ;
        //creation vertices
        std::map<Pointi, Pointd> vertices;
        double x, y, z;
        unsigned int i, j , k;

        // fix z - k = 0;
        k = 0; z = bb.getMinZ();
        x = bb.getMinX();
        for (i = 0; i <= nSubdX; i++){
            y = bb.getMinY();
            for (j = 0; j <= nSubdY; j++){
                Pointi pi(i,j,k);
                Pointd pd(x,y,z);
                vertices[pi] = pd;
                y+=edgeLengthY;
            }
            x+=edgeLengthX;
        }
        // fix z - k = nSubdZ;
        k = nSubdZ; z = bb.getMaxZ();
        x = bb.getMinX();
        for (i = 0; i <= nSubdX; i++){
            y = bb.getMinY();
            for (j = 0; j <= nSubdY; j++){
                Pointi pi(i,j,k);
                Pointd pd(x,y,z);
                vertices[pi] = pd;
                y+=edgeLengthY;
            }
            x+=edgeLengthX;
        }
        // fix y - j = 0;
        j = 0; y = bb.getMinY();
        x = bb.getMinX();
        for (i = 0; i <= nSubdX; i++){
            z = bb.getMinZ();
            for (k = 0; k <= nSubdZ; k++){
                Pointi pi(i,j,k);
                Pointd pd(x,y,z);
                vertices[pi] = pd;
                z+=edgeLengthZ;
            }
            x+=edgeLengthX;
        }
        // fix y - j = nSubdY;
        j = nSubdY; y = bb.getMaxY();
        x = bb.getMinX();
        for (i = 0; i <= nSubdX; i++){
            z = bb.getMinZ();
            for (k = 0; k <= nSubdZ; k++){
                Pointi pi(i,j,k);
                Pointd pd(x,y,z);
                vertices[pi] = pd;
                z+=edgeLengthZ;
            }
            x+=edgeLengthX;
        }
        // fix x - i = 0;
        i = 0; x = bb.getMinX();
        y = bb.getMinY();
        for (j = 0; j <= nSubdY; j++){
            z = bb.getMinZ();
            for (k = 0; k <= nSubdZ; k++){
                Pointi pi(i,j,k);
                Pointd pd(x,y,z);
                vertices[pi] = pd;
                z+=edgeLengthZ;
            }
            y+=edgeLengthY;
        }
        // fix x - i = nSubdX;
        i = nSubdX; x = bb.getMaxX();
        y = bb.getMinY();
        for (j = 0; j <= nSubdY; j++){
            z = bb.getMinZ();
            for (k = 0; k <= nSubdZ; k++){
                Pointi pi(i,j,k);
                Pointd pd(x,y,z);
                vertices[pi] = pd;
                z+=edgeLengthZ;
            }
            y+=edgeLengthY;
        }

        std::map<Pointi, int> indices;
        int index = 0;
        box.resizeVertices(vertices.size());
        for (std::pair<Pointi, Pointd> pair : vertices) {
            indices[pair.first] = index;
            box.setVertex(index, pair.second.x(), pair.second.y(), pair.second.z());
            index++;

        }

        //triangles
        // fix z - k = 0;
        k = 0;
        for (i = 0; i < nSubdX; i++){
            for (j = 0; j < nSubdY; j++){
                Pointi pi1(i,j,k);
                Pointi pi2(i+1,j,k);
                Pointi pi3(i+1,j+1,k);
                Pointi pi4(i,j+1,k);
                assert(indices.find(pi1) != indices.end());
                assert(indices.find(pi2) != indices.end());
                assert(indices.find(pi3) != indices.end());
                assert(indices.find(pi4) != indices.end());
                int i1 = indices[pi1], i2 = indices[pi2], i3 = indices[pi3], i4 = indices[pi4];
                box.addFace(i2, i1, i3);
                box.addFace(i3, i1, i4);
            }
        }
        // fix z - k = nSubdZ;
        k = nSubdZ;
        for (i = 0; i < nSubdX; i++){
            for (j = 0; j < nSubdY; j++){
                Pointi pi1(i,j,k);
                Pointi pi2(i+1,j,k);
                Pointi pi3(i+1,j+1,k);
                Pointi pi4(i,j+1,k);
                assert(indices.find(pi1) != indices.end());
                assert(indices.find(pi2) != indices.end());
                assert(indices.find(pi3) != indices.end());
                assert(indices.find(pi4) != indices.end());
                int i1 = indices[pi1], i2 = indices[pi2], i3 = indices[pi3], i4 = indices[pi4];
                box.addFace(i1, i2, i3);
                box.addFace(i1, i3, i4);
            }
        }
        // fix y - j = 0;
        j = 0;
        for (i = 0; i < nSubdX; i++){
            for (k = 0; k < nSubdZ; k++){
                Pointi pi1(i,j,k);
                Pointi pi2(i+1,j,k);
                Pointi pi3(i+1,j,k+1);
                Pointi pi4(i,j,k+1);
                assert(indices.find(pi1) != indices.end());
                assert(indices.find(pi2) != indices.end());
                assert(indices.find(pi3) != indices.end());
                assert(indices.find(pi4) != indices.end());
                int i1 = indices[pi1], i2 = indices[pi2], i3 = indices[pi3], i4 = indices[pi4];
                box.addFace(i1, i2, i3);
                box.addFace(i1, i3, i4);
            }
        }
        // fix y - j = nSubdY;
        j = nSubdY;
        for (i = 0; i < nSubdX; i++){
            for (k = 0; k < nSubdZ; k++){
                Pointi pi1(i,j,k);
                Pointi pi2(i+1,j,k);
                Pointi pi3(i+1,j,k+1);
                Pointi pi4(i,j,k+1);
                assert(indices.find(pi1) != indices.end());
                assert(indices.find(pi2) != indices.end());
                assert(indices.find(pi3) != indices.end());
                assert(indices.find(pi4) != indices.end());
                int i1 = indices[pi1], i2 = indices[pi2], i3 = indices[pi3], i4 = indices[pi4];
                box.addFace(i2, i1, i3);
                box.addFace(i3, i1, i4);
            }
        }
        // fix x - i = 0;
        i = 0;
        for (j = 0; j < nSubdY; j++){
            for (k = 0; k < nSubdZ; k++){
                Pointi pi1(i,j,k);
                Pointi pi2(i,j+1,k);
                Pointi pi3(i,j+1,k+1);
                Pointi pi4(i,j,k+1);
                assert(indices.find(pi1) != indices.end());
                assert(indices.find(pi2) != indices.end());
                assert(indices.find(pi3) != indices.end());
                assert(indices.find(pi4) != indices.end());
                int i1 = indices[pi1], i2 = indices[pi2], i3 = indices[pi3], i4 = indices[pi4];
                box.addFace(i2, i1, i3);
                box.addFace(i3, i1, i4);
            }
        }
        // fix x - i = nSubdX;
        i = nSubdX;
        for (j = 0; j < nSubdY; j++){
            for (k = 0; k < nSubdZ; k++){
                Pointi pi1(i,j,k);
                Pointi pi2(i,j+1,k);
                Pointi pi3(i,j+1,k+1);
                Pointi pi4(i,j,k+1);
                assert(indices.find(pi1) != indices.end());
                assert(indices.find(pi2) != indices.end());
                assert(indices.find(pi3) != indices.end());
                assert(indices.find(pi4) != indices.end());
                int i1 = indices[pi1], i2 = indices[pi2], i3 = indices[pi3], i4 = indices[pi4];
                box.addFace(i1, i2, i3);
                box.addFace(i1, i3, i4);
            }
        }
    }
    return box;
}

#ifdef IGL_DEFINED
SimpleEigenMesh EigenMeshAlgorithms::decimateMesh(const SimpleEigenMesh& m, int numberDesiredFaces, Eigen::VectorXi& mapping) {
    SimpleEigenMesh output;

    /**
     * @todo Pull request
     */
    Eigen::MatrixXd VV;
    Eigen::MatrixXi FF;
    igl::decimate(m.V, m.F, numberDesiredFaces, VV, FF, mapping);

    output.V = VV;
    output.F = FF;
    return output;
}

EigenMesh EigenMeshAlgorithms::decimateMesh(const EigenMesh& m, int numberDesiredFaces, Eigen::VectorXi& mapping) {
    EigenMesh output;

    /**
     * @todo Pull request
     */
    Eigen::MatrixXd VV;
    Eigen::MatrixXi FF;
    igl::decimate(m.V, m.F, numberDesiredFaces, VV, FF, mapping);

    output.V = VV;
    output.F = FF;
    output.updateFaceNormals();
    output.updateVerticesNormals();
    output.CV = Eigen::MatrixXf::Constant(output.V.rows(), 3, 0.5);
    output.CF.resize(output.F.rows(), 3);
    for (unsigned int i = 0; i < output.F.rows(); i++){
        output.CF(i,0) = m.CF(mapping(i), 0);
        output.CF(i,1) = m.CF(mapping(i), 1);
        output.CF(i,2) = m.CF(mapping(i), 2);
    }
    output.updateBoundingBox();

    return output;
}

void EigenMeshAlgorithms::decimateMesh(SimpleEigenMesh& m, int numberDesiredFaces, Eigen::VectorXi& mapping) {
    /**
     * @todo Pull request
     */
    Eigen::MatrixXd VV;
    Eigen::MatrixXi FF;
    igl::decimate(m.V, m.F, numberDesiredFaces, VV, FF, mapping);
    m.V = VV;
    m.F = FF;
}

void EigenMeshAlgorithms::decimateMesh(EigenMesh& m, int numberDesiredFaces, Eigen::VectorXi& mapping) {
    Eigen::MatrixXd VV;
    Eigen::MatrixXi FF;
    igl::decimate(m.V, m.F, numberDesiredFaces, VV, FF, mapping);
    m.V = VV;
    m.F = FF;

    m.CV = Eigen::MatrixXf::Constant(m.V.rows(), 3, 0.5);
    Eigen::MatrixXf tmp = m.CF;
    m.CF.resize(m.F.rows(), 3);
    for (int i = FF.rows()-1; i>= 0; i--){
        m.CF(i,0) = tmp(mapping(i), 0);
        m.CF(i,1) = tmp(mapping(i), 1);
        m.CF(i,2) = tmp(mapping(i), 2);
    }

    m.updateBoundingBox();
}

#ifdef CGAL_DEFINED
void EigenMeshAlgorithms::intersection(SimpleEigenMesh& result, const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    igl::copyleft::cgal::CSGTree M;
    M = {{m1.V,m1.F},{m2.V,m2.F},"i"};
    result.V = M.cast_V<Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> >();
    result.F = M.F();
}

void EigenMeshAlgorithms::intersection(EigenMesh& result, const EigenMesh& m1, const EigenMesh& m2) {
    SimpleEigenMesh sres;
    EigenMeshAlgorithms::intersection(sres, SimpleEigenMesh(m1.V, m1.F), SimpleEigenMesh(m2.V, m2.F));
    result = EigenMesh(sres);
    result.updateColorSizes();
    result.NV.resize(result.V.rows(), 3);
    result.NF.resize(result.F.rows(), 3);
    result.updateFaceNormals();
    result.updateVerticesNormals();
}

SimpleEigenMesh EigenMeshAlgorithms::intersection(const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    SimpleEigenMesh result;
    igl::copyleft::cgal::CSGTree M;
    M = {{m1.V,m1.F},{m2.V,m2.F},"i"};
    result.V = M.cast_V<Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> >();
    result.F = M.F();
    return result;
}

EigenMesh EigenMeshAlgorithms::intersection(const EigenMesh& m1, const EigenMesh& m2) {
    EigenMesh result;
    SimpleEigenMesh sres;
    EigenMeshAlgorithms::intersection(sres, SimpleEigenMesh(m1.V, m1.F), SimpleEigenMesh(m2.V, m2.F));
    result = EigenMesh(sres);
    result.updateColorSizes();
    result.NV.resize(result.V.rows(), 3);
    result.NF.resize(result.F.rows(), 3);
    result.updateFaceNormals();
    result.updateVerticesNormals();
    return result;
}

void EigenMeshAlgorithms::difference(SimpleEigenMesh& result, const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    igl::copyleft::cgal::CSGTree M;
    M = {{m1.V,m1.F},{m2.V,m2.F},"m"};
    result.V = M.cast_V<Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> >();
    result.F = M.F();
}

void EigenMeshAlgorithms::difference(EigenMesh& result, const EigenMesh& m1, const EigenMesh& m2) {
    SimpleEigenMesh sres;
    EigenMeshAlgorithms::difference(sres, SimpleEigenMesh(m1.V, m1.F), SimpleEigenMesh(m2.V, m2.F));
    result = EigenMesh(sres);
    result.updateColorSizes();
    result.NV.resize(result.V.rows(), 3);
    result.NF.resize(result.F.rows(), 3);
    result.updateFaceNormals();
    result.updateVerticesNormals();
}

SimpleEigenMesh EigenMeshAlgorithms::difference(const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    SimpleEigenMesh result;
    igl::copyleft::cgal::CSGTree M;
    M = {{m1.V,m1.F},{m2.V,m2.F},"m"};
    result.V = M.cast_V<Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> >();
    result.F = M.F();
    return result;
}

EigenMesh EigenMeshAlgorithms::difference(const EigenMesh& m1, const EigenMesh& m2) {
    EigenMesh result;
    SimpleEigenMesh sres;
    EigenMeshAlgorithms::difference(sres, SimpleEigenMesh(m1.V, m1.F), SimpleEigenMesh(m2.V, m2.F));
    result = EigenMesh(sres);
    result.updateColorSizes();
    result.NV.resize(result.V.rows(), 3);
    result.NF.resize(result.F.rows(), 3);
    result.updateFaceNormals();
    result.updateVerticesNormals();
    return result;
}

void EigenMeshAlgorithms::union_(SimpleEigenMesh& result, const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    igl::copyleft::cgal::CSGTree M;
    M = {{m1.V,m1.F},{m2.V,m2.F},"u"};
    result.V = M.cast_V<Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> >();
    result.F = M.F();
}

void EigenMeshAlgorithms::union_(EigenMesh& result, const EigenMesh& m1, const EigenMesh& m2) {
    SimpleEigenMesh sres;
    EigenMeshAlgorithms::union_(sres, SimpleEigenMesh(m1.V, m1.F), SimpleEigenMesh(m2.V, m2.F));
    result = EigenMesh(sres);
    result.updateColorSizes();
    result.NV.resize(result.V.rows(), 3);
    result.NF.resize(result.F.rows(), 3);
    result.updateFaceNormals();
    result.updateVerticesNormals();
}

SimpleEigenMesh EigenMeshAlgorithms::union_(const SimpleEigenMesh& m1, const SimpleEigenMesh& m2) {
    SimpleEigenMesh result;
    igl::copyleft::cgal::CSGTree M;
    M = {{m1.V,m1.F},{m2.V,m2.F},"u"};
    result.V = M.cast_V<Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> >();
    result.F = M.F();
    return result;
}

EigenMesh EigenMeshAlgorithms::union_(const EigenMesh& m1, const EigenMesh& m2) {
    EigenMesh result;
    SimpleEigenMesh sres;
    EigenMeshAlgorithms::union_(sres, SimpleEigenMesh(m1.V, m1.F), SimpleEigenMesh(m2.V, m2.F));
    result = EigenMesh(sres);
    result.updateColorSizes();
    result.NV.resize(result.V.rows(), 3);
    result.NF.resize(result.F.rows(), 3);
    result.updateFaceNormals();
    result.updateVerticesNormals();
    return result;
}

void EigenMeshAlgorithms::removeUnreferencedVertices(SimpleEigenMesh& input, Eigen::Matrix<int, Eigen::Dynamic, 1> &I) {
    //Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::RowMajor> NV;
    //Eigen::Matrix<int, Eigen::Dynamic, 3, Eigen::RowMajor> NF;
    //igl::remove_unreferenced(V,F, NV, NF, I);

    /**
      @todo Pull request
    */
    ///
    /// If IGL Static (pull request problem)
    Eigen::MatrixXd VV = input.V, NV;
    Eigen::MatrixXi FF = input.F, NF;
    igl::remove_unreferenced(VV,FF, NV, NF, I);
    ///

    input.V = NV;
    input.F = NF;
}

std::vector<SimpleEigenMesh> EigenMeshAlgorithms::getConnectedComponents(const SimpleEigenMesh& mm) {
    std::vector<SimpleEigenMesh> connectedComponents;
    Eigen::VectorXi C;
    //igl::facet_components(F, C);

    /**
      @todo Pull request
    */
    ///If IGL Static (pull request problem)
    Eigen::MatrixXi FF = mm.F;
    igl::facet_components(FF, C);
    ///
    for (unsigned int i = 0; i < C.size(); i++){
        if (C(i) >= (int)connectedComponents.size()){
            assert(C(i) == (int)connectedComponents.size());
            SimpleEigenMesh m;
            m.V = mm.V;
            connectedComponents.push_back(m);
        }
        connectedComponents[C(i)].addFace(mm.F.row(i));
    }
    for (unsigned int i = 0; i < connectedComponents.size(); i++){
        Eigen::Matrix<int, Eigen::Dynamic, 1> I;
        removeUnreferencedVertices(connectedComponents[i], I);
    }
    return connectedComponents;
}

Eigen::MatrixXi EigenMeshAlgorithms::getFaceAdjacences(const SimpleEigenMesh& m) {
    Eigen::MatrixXi TT;
    //igl::triangle_triangle_adjacency(F, TT);
    /**
     * @todo Pull request libigl
     */
    Eigen::MatrixXi FF = m.F;
    igl::triangle_triangle_adjacency(FF, TT);
    return TT;
}
#endif

#endif
