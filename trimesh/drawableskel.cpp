#include "drawableskel.h"

DrawableSkel::DrawableSkel()
{

}

DrawableSkel::~DrawableSkel()
{
    edge_list = CgalSkeletonInterface::computeSkeleton("\home\Desktop\armadillofattocondcel.obj");
}

bool DrawableSkel::isVisible() const
{
    return true;
}

double DrawableSkel::sceneRadius() const
{
    return 0.0;
}

Pointd DrawableSkel::sceneCenter() const
{
    return Pointd(0,0,0);
}

void DrawableSkel::draw() const
{
    for(auto it = edge_list.begin(); it != edge_list.end(); ++it)
    {
        drawSphere((*it).first,1,QColor(0,255,0,127));
        drawSphere((*it).second,1,QColor(0,255,0,127));
    }

    for(auto it = edge_list.begin(); it != edge_list.end(); ++it)
    {
        drawCylinder((*it).first, (*it).second, 1, 1, QColor(127,127,127,127));
    }
}
