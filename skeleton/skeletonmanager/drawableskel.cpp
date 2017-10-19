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

void DrawableSkel::compressSkeletonUntilConverge()
{
    buildAdjacency();
    CompactTrisOnBoarders compacter(tris, coords, triTrashold, tri2tri, vtx2tri );
    buildAdjacency();
    saveObj("//home//pietro//Desktop//meshCompattata.obj",coords,tris);


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
    int n_tris = tris.size()/3;
    for(int tid=0; tid<n_tris; ++tid)
    {
        int tid_ptr  = 3 * tid;
        int vid0     = tris[tid_ptr + 0];
        int vid1     = tris[tid_ptr + 1];
        int vid2     = tris[tid_ptr + 2];
        int vid0_ptr = 3 * vid0;
        int vid1_ptr = 3 * vid1;
        int vid2_ptr = 3 * vid2;
        int j=0;
        Pointd a(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]);
        Pointd b(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]);
        Pointd c(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2]);

        haveNeighborsSmallArea = true;
        if(isTrisOnBorder2(a,b,c,triTrashold))
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

            }

            //altrimenti coloro di rosso
            else
            {
                glBegin(GL_TRIANGLES);

                glColor3f(1.0, 0.0, 0.0);}
            }
        else
        {
            glBegin(GL_TRIANGLES);
            glColor3f(0.1, 0.8, 0.1);
        }

        glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid0_ptr]));
        glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid1_ptr]));
        glNormal3dv(&(triangleNormals[tid_ptr]));
        glVertex3dv(&(coords[vid2_ptr]));
        glEnd();

        drawLine(a,b,QColor(0,0,0,127));
        drawLine(a,c,QColor(0,0,0,127));
        drawLine(c,b,QColor(0,0,0,127));
    }

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

/**
 * @brief DrawableSkel::minDistanceBetweenThreePoints calculate minimum distance between points
 * @param a
 * @param b
 * @param c
 * @return 0 if the edge is first-second, 1 if it is first third, -1 if it is second third
 */
int DrawableSkel::minDistanceBetweenThreePoints(Pointd a, Pointd b, Pointd c)
{
    double distanceAB = a.dist(b);
    double distanceAC = a.dist(c);
    double distanceBC = b.dist(c);

    if(distanceAB < distanceAC)
    {
        if(distanceAB < distanceBC)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        if(distanceAC < distanceBC)
        {
            return 1;

        }
        else
        {
            return -1;
        }
    }


}

/**
 * @brief DrawableSkel::hasTidThisTwoVertexes
 * @param tid
 * @param a
 * @param b
 * @return true se tid ha a e b come vertici, false altrimenti.
 */
bool DrawableSkel::hasTidThisTwoVertexes(int tid, Pointd a, Pointd b)
{
    int tid_ptr  = 3 * tid;
    int vid0     = tris[tid_ptr + 0];
    int vid1     = tris[tid_ptr + 1];
    int vid2     = tris[tid_ptr + 2];
    int vid0_ptr = 3*vid0;
    int vid1_ptr = 3*vid1;
    int vid2_ptr = 3*vid2;
    Pointd tA(coords[vid0_ptr+0],coords[vid0_ptr+1],coords[vid0_ptr+2]);
    Pointd tB(coords[vid1_ptr+0],coords[vid1_ptr+1],coords[vid1_ptr+2]);
    Pointd tC(coords[vid2_ptr+0],coords[vid2_ptr+1],coords[vid2_ptr+2]);

    if(tA == a)
    {
        if(tB == b || tC == b)
        {
            return true;

        }
        else
        {
            return false;
        }
    }
    else
    {
        if(tA == b)
        {
            if(tB == a || tC == a)
            {
                return true;

            }
            else
            {
                return false;
            }
        }
        if(tB == a)
        {
            if(tC == b)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(tB == b)
            {
                if(tC == a)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }

}
