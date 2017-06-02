#include "drawableskel.h"

DrawableSkel::DrawableSkel()
{
    init();
}

DrawableSkel::DrawableSkel(const char *file_name)
{
    init();
    SkeletonCreator skelCreator(file_name);
    edge_list = skelCreator.getListaEdge();
    tris = skelCreator.getListaTriangoli();
    coords = skelCreator.getListaPunti();

}

DrawableSkel::DrawableSkel(const Skel *skel)
{
    skeleton = skel;
}

DrawableSkel::~DrawableSkel()
{

}

bool DrawableSkel::isVisible() const
{
    return true;
}

void DrawableSkel::init()
{
    DrawableMesh::init();
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
    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glDepthRange(0.01, 1.0);

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

    int n_tris = tris.size()/3;
    for(int tid=0; tid<n_tris; ++tid) {
        int tid_ptr  = 3 * tid;
        int vid0     = tris[tid_ptr + 0];
        int vid1     = tris[tid_ptr + 1];
        int vid2     = tris[tid_ptr + 2];
        int vid0_ptr = 3 * vid0;
        int vid1_ptr = 3 * vid1;
        int vid2_ptr = 3 * vid2;
        if(isTrisOnBorder(Pointd(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]),
                          Pointd(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]),
                          Pointd(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2])))
        {
            glBegin(GL_TRIANGLES);

            glColor3f(1.0, 0.0, 0.0);
        }
        else
        {
            glBegin(GL_TRIANGLES);
            glColor3f(0.1, 0.8, 0.1);
        }
        //glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid0_ptr]));
        //glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid1_ptr]));
        //glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid2_ptr]));
        glEnd();
    }
      //drawSphere(edge_list->front().first,1,QColor(0,255,0,127),20);

}

void DrawableSkel::setEdgeList(std::list<std::pair<Pointd, Pointd>> edges)
{
    edge_list = edges;
}

bool DrawableSkel::isTrisOnBorder(Pointd a, Pointd b, Pointd c)
{
    double soglia = 3.14159265/18.0;

    Pointd v1 (a.x() - b.x(), a.y() - b.y(), a.z() - b.z() );
    Pointd v2 (b.x() - c.x(), b.y() - c.y(), b.z() - c.z() );
    Pointd v3 (a.x() - c.x(), a.y() - c.y(), a.z() - c.z() );
    v1.normalize();
    v2.normalize();
    v3.normalize();

    double angle1 = acos(v1.dot(v2));
    double angle2 = acos(v1.dot(v3));
    double angle3 = acos(v2.dot(v3));


    //qDebug() << "angolo1 " << angle1 << " angolo2 " << angle2 << " angolo3 " << angle3 << "\n";

    if(angle1 < soglia || angle2 <soglia || angle3 < soglia)
    {
        return true;
    }
    else
    {
        return false;
    }


}

