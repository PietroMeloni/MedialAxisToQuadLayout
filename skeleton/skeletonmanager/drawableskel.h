#ifndef DRAWABLESKEL_H
#define DRAWABLESKEL_H

#include <math.h>
#include "viewer/interfaces/drawable_object.h"
#include "viewer/interfaces/drawable_mesh.h"
#include "skel.h"
#include "viewer/objects/objects.h"
#include <QColor>
#include "viewer/mainwindow.h"
#include "skeleton/skeletoncreator.h"
#include "trimesh/trimesh.h"

class DrawableSkel : public DrawableMesh,
                     public Trimesh<double>
{
    public:
        DrawableSkel();
        DrawableSkel(const char* file_name);
        DrawableSkel(const Skel *skel);

        ~DrawableSkel();
        // Implementation of the
        // DrawableObject interface
        void draw() const;
        Pointd sceneCenter() const ;
        double sceneRadius() const ;

        bool isVisible() const;
        void init();



   protected:

        //std::list<Pointd> vtx_list;
        std::list<std::pair<Pointd, Pointd>> edge_list;
        const Skel* skeleton;
        MainWindow* mainWindow;
        DrawableTrimesh* trimesh;
        DrawableSkel* dsk;


        void setEdgeList(std::list<std::pair<Pointd, Pointd>> edges);
        static bool isTrisOnBorder(Pointd a, Pointd b, Pointd c);

};

#endif // DRAWABLESKEL_H
