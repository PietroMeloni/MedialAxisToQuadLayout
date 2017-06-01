#include "drawableeigenmesh.h"

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

DrawableEigenMesh::DrawableEigenMesh() {
}

DrawableEigenMesh::DrawableEigenMesh(const SimpleEigenMesh& m) : EigenMesh(m) {
}

DrawableEigenMesh::DrawableEigenMesh(const EigenMesh& m) : EigenMesh(m){
}

DrawableEigenMesh::~DrawableEigenMesh(){
}

void DrawableEigenMesh::draw() const {
    DrawableMesh::draw(V.rows(), F.rows(), V.data(), F.data(), NV.data(), CV.data(), NF.data(), CF.data());
}

Pointd DrawableEigenMesh::sceneCenter() const {
    return bb.center();
}

double DrawableEigenMesh::sceneRadius() const {
    return bb.diag();
}
