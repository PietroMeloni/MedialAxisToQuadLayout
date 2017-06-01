#ifndef DRAWABLESKEL_H
#define DRAWABLESKEL_H

#include "viewer/interfaces/drawable_object.h"
#include "skel.h"
#include "viewer/objects/objects.h"
#include <QColor>


class DrawableSkel : public DrawableObject
{
    public:
        DrawableSkel();

        virtual ~DrawableSkel();
        // Implementation of the
        // DrawableObject interface
        virtual void draw() const = 0;
        virtual Pointd sceneCenter() const = 0;
        virtual double sceneRadius() const = 0;
        virtual bool isVisible() const;

   protected:

        //std::list<Pointd> vtx_list;
        std::list<std::pair<Pointd, Pointd>> edge_list;


};

#endif // DRAWABLESKEL_H
