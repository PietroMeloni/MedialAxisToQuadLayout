#ifndef DRAWABLESKEL_H
#define DRAWABLESKEL_H

#include "common/trischar.h"
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

        const int TID_ELIM = -5;


        DrawableSkel();
        DrawableSkel(const char* file_name);
        DrawableSkel(const char* file_name, int compressions, double trashold);
        DrawableSkel(const Skel *skel);

        ~DrawableSkel();
        // Implementation of the
        // DrawableObject interface
        void draw() const;
        Pointd sceneCenter() const ;
        double sceneRadius() const ;

        bool isVisible() const;
        void init();
        void compressSkeletonUntilConverge();



   protected:

        //std::list<Pointd> vtx_list;
        std::list<std::pair<Pointd, Pointd>> edge_list;
        const Skel* skeleton;
        MainWindow* mainWindow;
        DrawableTrimesh* trimesh;
        DrawableSkel* dsk;
        double triTrashold;


        void setEdgeList(std::list<std::pair<Pointd, Pointd>> edges);
        static bool isTrisOnBorder(Pointd a, Pointd b, Pointd c, double trasholdAngle);
        static bool isTrisOnBorder2(Pointd a, Pointd b, Pointd c, double trasholdAngle);
        static bool isTrisOnBorder3(Pointd a, Pointd b, Pointd c, double trasholdAngle);
        bool mergeTwoVertexes(int tid);
        bool mergeTwoVertexes(int tid, Pointd a, Pointd b);
        bool shiftTriangleList();
        bool shiftTriangleList(int tid);
        bool deleteTriangle(int tid);
        int minDistanceBetweenThreePoints(Pointd a, Pointd b, Pointd c);
        bool hasTidThisTwoVertexes(int tid, Pointd a, Pointd b);
        bool deleteVTX(int vid);


};

#endif // DRAWABLESKEL_H
