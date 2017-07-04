#include "drawableskel.h"

DrawableSkel::DrawableSkel()
{
    init();
}

DrawableSkel::DrawableSkel(const char *file_name)
{
    init();
    triTrashold = 10;
    SkeletonCreator skelCreator(file_name);
    edge_list = skelCreator.getListaEdge();
    tris = skelCreator.getListaTriangoli();
    coords = skelCreator.getListaPunti();
    buildAdjacency();
    updateNormals();
    updateBbox();

}

DrawableSkel::DrawableSkel(const char *file_name, int compr, double trashold)
{
    init();
    SkeletonCreator skelCreator(file_name, compr);


    triTrashold = trashold;
    edge_list = skelCreator.getListaEdge();
    tris = skelCreator.getListaTriangoli();
    coords = skelCreator.getListaPunti();
    buildAdjacency();
    updateNormals();
    updateBbox();

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
    return bbox.diag();
}

Pointd DrawableSkel::sceneCenter() const
{
    Pointd c = bbox.center();
    return Pointd(c.x(), c.y(), c.z());
}

void DrawableSkel::draw() const
{
    QList<Pointd> temp;
    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);
    glDepthRange(0.01, 1.0);
    bool haveNeighborsSmallArea = true;

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
        int j=0;
        haveNeighborsSmallArea = true;
        if(isTrisOnBorder2(Pointd(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]),
                          Pointd(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]),
                          Pointd(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2]),
                          triTrashold)||
           isTrisOnBorder(Pointd(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]),
                           Pointd(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]),
                           Pointd(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2]),
                           15))
        {

            //controllo se i vicini hanno area piccola anche loro tramite
            //i vettori tri2tri "riempiti" da metodi già esistenti come
            //buildAdjagency. In questo caso, se un triangolo ha almeno 2 vicini
            //che hanno area piccola, allora lo coloro di rosso, altrimenti di blu.
            for(int i=0; i < tri2tri[tid].size(); i++)
            {

                haveNeighborsSmallArea = true;
                int tid_ptr0  = 3 * (tri2tri[tid])[i];
                int vid00     = tris[tid_ptr0 + 0];
                int vid10     = tris[tid_ptr0 + 1];
                int vid20     = tris[tid_ptr0 + 2];
                int vid0_ptr0 = 3 * vid00;
                int vid1_ptr0 = 3 * vid10;
                int vid2_ptr0 = 3 * vid20;
                //controllo l'area del vicino, presuppongo che i vicini siano 3 (constatato che non è sicuro)
                if(!isTrisOnBorder2(Pointd(coords[vid0_ptr0],coords[vid0_ptr0+1],coords[vid0_ptr0+2]),
                                  Pointd(coords[vid1_ptr0],coords[vid1_ptr0+1],coords[vid1_ptr0+2]),
                                  Pointd(coords[vid2_ptr0],coords[vid2_ptr0+1],coords[vid2_ptr0+2]),
                                    triTrashold))
                {

                    j++;
                }

                if(j > 1)
                {
                    haveNeighborsSmallArea = false;
                }
            }

            //se i vicini che hanno area piccola sono almeno 2 coloro di blu
            if(haveNeighborsSmallArea)
            {
                glBegin(GL_TRIANGLES);

                glColor3f(0.0, 0.0, 1.0);

                //qDebug()<< "tri2tri[ "<<tid <<"] = "<< tri2tri[tid] << ". Size= " << tri2tri[tid].size()<<"\n" ;
            }
            //altrimenti coloro di rosso
            else
            {
                glBegin(GL_TRIANGLES);

                glColor3f(1.0, 0.0, 0.0);
                //qDebug()<< "red";
                //qDebug()<< "tri2tri[ "<<tid <<"] = "<< tri2tri[tid] << ". Size= " << tri2tri[tid].size()<<"\n" ;
            }
        }
        else
        {
            //qDebug()<< "green";
            glBegin(GL_TRIANGLES);
            glColor3f(0.1, 0.8, 0.1);
        }
        //qDebug()<< "j = "<<j;
        //qDebug()<< "\n";
        glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid0_ptr]));
        glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid1_ptr]));
        glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid2_ptr]));
        glEnd();
    }
      //drawSphere(edge_list->front().first,1,QColor(0,255,0,127),20);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glDepthRange(0.0, 1.0);
//    glDepthFunc(GL_LEQUAL);
//    glDepthFunc(GL_LESS);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}

void DrawableSkel::setEdgeList(std::list<std::pair<Pointd, Pointd>> edges)
{
    edge_list = edges;
}


//usando l'angolo minimo
bool DrawableSkel::isTrisOnBorder(Pointd a, Pointd b, Pointd c, double trasholdAngle)
{
    double soglia = (3.14159265/180.0)*trasholdAngle;
    double minAngle = trisCharacteristic::getTriangleMinAngle(a,b,c);

    if(soglia > minAngle)
    {
        return true;
    }
    else
    {
        return false;
    }


}
//usando l'area minima
bool DrawableSkel::isTrisOnBorder2(Pointd a, Pointd b, Pointd c, double trashold)
{
    double soglia = trashold ;


    double area = trisCharacteristic::getTriangleArea(a,b,c);


    if(area < soglia)
    {
        return true;
    }
    else
    {
        return false;
    }


}

bool DrawableSkel::isTrisOnBorder3(Pointd a, Pointd b, Pointd c, double trashold)
{
    double soglia = trashold ;

//    qDebug()<< "Punti: "<< a.x() << " "<< a.y() << " "<< a.z()
//            << ";      "<< b.x() << " "<< b.y() << " "<< b.z()
//            << ";      "<< c.x() << " "<< c.y() << " "<< c.z();


    Pointd v1 (a.x() - b.x(), a.y() - b.y(), a.z() - b.z() );
    Pointd v2 (b.x() - c.x(), b.y() - c.y(), b.z() - c.z() );
    Pointd v3 (a.x() - c.x(), a.y() - c.y(), a.z() - c.z() );
    v1.normalize();
    v2.normalize();
    v3.normalize();

    double angle1 = acos(v1.dot(v2));
    double angle2 = acos(v1.dot(v3));
    double angle3 = acos(v2.dot(v3));


    double area = a.dist(b)*a.dist(c)*sin(angle2)/2;

    //qDebug() << "Area = " <<area;

    if(area < soglia)
    {
        return true;
    }
    else
    {
        return false;
    }


}
