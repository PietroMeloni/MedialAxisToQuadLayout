#ifndef DRAWABLESKEL_H
#define DRAWABLESKEL_H

#include "viewer/interfaces/drawable_object.h"
#include "skel.h"
#include "viewer/objects/objects.h"
#include <QColor>
#include "viewer/mainwindow.h"
#include "cgalskeletoninterface.h"

struct skelComponents {
    std::list<std::pair<Pointd, Pointd> > listaEdge;
    std::vector<int> listaTriangoli;
    std::vector<double> listaPunti;
};

class DrawableSkel : public DrawableObject
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



   protected:

        //std::list<Pointd> vtx_list;
        std::list<std::pair<Pointd, Pointd>> edge_list;
        const Skel* skeleton;
        MainWindow* mainWindow;
        DrawableTrimesh* trimesh;
        DrawableSkel* dsk;

        void setEdgeList(std::list<std::pair<Pointd, Pointd>> edges);



};

#endif // DRAWABLESKEL_H
