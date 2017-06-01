#ifndef DRAWABLEEIGENMESH_H
#define DRAWABLEEIGENMESH_H

#include <viewer/interfaces/drawable_mesh.h>
#include "../eigenmesh.h"
class DrawableEigenMesh : public EigenMesh, public DrawableMesh {
    public:
        DrawableEigenMesh();
        DrawableEigenMesh(const SimpleEigenMesh &m);
        DrawableEigenMesh(const EigenMesh &m);
        virtual ~DrawableEigenMesh();

        // DrawableObject interface
        void draw() const;
        Pointd sceneCenter() const;
        double sceneRadius() const;
};

#endif // DRAWABLEEIGENMESH_H
