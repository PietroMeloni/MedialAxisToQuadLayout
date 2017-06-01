#include "drawableskel.h"

DrawableSkel::DrawableSkel()
{

        //edge_list = CgalSkeletonInterface::computeSkeleton(filename.toStdString().c_str());


}

DrawableSkel::DrawableSkel(const char *file_name)
{
    edge_list = CgalSkeletonInterface::computeSkeleton(file_name);
}

DrawableSkel::DrawableSkel(const Skel *skel)
{
    skeleton = skel;
   // edge_list = CgalSkeletonInterface::computeSkeleton();

}

DrawableSkel::~DrawableSkel()
{

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
    QList<Pointd> temp;

    for(std::pair<Pointd, Pointd> pair : edge_list)
    {
//        if(!temp.contains(pair.first))
//        {
//            drawSphere(pair.first,0.005,QColor(0,255,0,127));
//            temp.push_back(pair.first);
//        }
//        if(!temp.contains(pair.second))
//        {
//            drawSphere(pair.second,0.005,QColor(0,255,0,127));
//            temp.push_back(pair.second);
//        }
        drawLine(pair.first, pair.second, QColor(127,127,127,127));
    }



      //drawSphere(edge_list->front().first,1,QColor(0,255,0,127),20);

}

void DrawableSkel::setEdgeList(std::list<std::pair<Pointd, Pointd>> edges)
{
    edge_list = edges;
}
