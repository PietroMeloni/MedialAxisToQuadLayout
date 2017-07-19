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

    for(int tid=0; tid< tris.size()/3; ++tid)
    {
        int tid_ptr  = 3 * tid;
        int vid0     = tris[tid_ptr + 0];
        int vid1     = tris[tid_ptr + 1];
        int vid2     = tris[tid_ptr + 2];
        int vid0_ptr = 3 * vid0;
        int vid1_ptr = 3 * vid1;
        int vid2_ptr = 3 * vid2;

        if(isTrisOnBorder2(Pointd(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]),
                          Pointd(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]),
                          Pointd(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2]),
                          triTrashold)||
           isTrisOnBorder(Pointd(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]),
                           Pointd(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]),
                           Pointd(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2]),
                           15))
        {

            mergeTwoVertexes(tid);
            shiftTriangleList();
            buildAdjacency();
            tid--;

        }

    }
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
/**
 * @brief DrawableSkel::mergeTwoVertexes merge 2 points and deletes 2 triangles.
 * @param tid id of the triangle that will be deleted with one of his neighbours
 * @return true if we can merge, false otherwise
 */
bool DrawableSkel::mergeTwoVertexes(int tid)
{
    int tid_ptr  = 3 * tid;
    int vid0     = tris[tid_ptr + 0];
    int vid1     = tris[tid_ptr + 1];
    int vid2     = tris[tid_ptr + 2];
    int vid0_ptr = 3 * vid0;
    int vid1_ptr = 3 * vid1;
    int vid2_ptr = 3 * vid2;
    Pointd vA(coords[vid0_ptr+0], coords[vid0_ptr+1], coords[vid0_ptr+2]);
    Pointd vB(coords[vid1_ptr+0], coords[vid1_ptr+1], coords[vid1_ptr+2]);
    Pointd vC(coords[vid2_ptr+0], coords[vid2_ptr+1], coords[vid2_ptr+2]);


    int neighboursNull = 0;
    bool found = false;

    if(tri2tri[tid].size() != 0)
    {

        for(int i=0; i < tri2tri[tid].size()&&!found; i++)
        {
            int tid_ptr0  = 3 * (tri2tri[tid])[i];
            if( tid_ptr0 >= 0)
            {
                int vid00     = tris[tid_ptr0 + 0];
                int vid10     = tris[tid_ptr0 + 1];
                int vid20     = tris[tid_ptr0 + 2];
                int vid0_ptr0 = 3 * vid00;
                int vid1_ptr0 = 3 * vid10;
                int vid2_ptr0 = 3 * vid20;

                //provo a cercare se uno dei primi due triangoli vicini è nel bordo.
                //Se lo è, uso il vertice di quel triangolo come vertice nel quale
                //unire quello che devo cancellare. Il punto cancellato diventerà
                //un doppione di un altro nella lista (in teoria non dovrebbe disturbare).
                if(i < 3)
                {
                    if(vid00 == vid0 || vid00 == vid1 || vid00 == vid2)
                    {
                        if(vid10 == vid0 || vid10 == vid1 || vid10 == vid2)
                        {
                            deleteTriangle((tri2tri[tid])[i]);
                            deleteTriangle(tid);
                            //shiftTriangleList(tid);
                            //shiftTriangleList((tri2tri[tid])[i]);
                            coords[vid0_ptr0 + 0] = coords[vid1_ptr0 + 0];
                            coords[vid0_ptr0 + 1] = coords[vid1_ptr0 + 1];
                            coords[vid0_ptr0 + 2] = coords[vid1_ptr0 + 2];
                            found = true;
                        }
                        else
                        {
                            if(vid20 == vid0 || vid20 == vid1 || vid20 == vid2)
                            {
                                deleteTriangle((tri2tri[tid])[i]);
                                deleteTriangle(tid);
                                //shiftTriangleList(tid);
                                //shiftTriangleList((tri2tri[tid])[i]);
                                coords[vid0_ptr0 + 0] = coords[vid2_ptr0 + 0];
                                coords[vid0_ptr0 + 1] = coords[vid2_ptr0 + 1];
                                coords[vid0_ptr0 + 2] = coords[vid2_ptr0 + 2];
                                found = true;
                            }
                        }

                    }
                    else
                    {
                        if(vid10 == vid0 || vid10 == vid1 || vid10 == vid2)
                        {
                            deleteTriangle((tri2tri[tid])[i]);
                            deleteTriangle(tid);
                            //shiftTriangleList(tid);
                            //shiftTriangleList((tri2tri[tid])[i]);
                            coords[vid1_ptr0 + 0] = coords[vid2_ptr0 + 0];
                            coords[vid1_ptr0 + 1] = coords[vid2_ptr0 + 1];
                            coords[vid1_ptr0 + 2] = coords[vid2_ptr0 + 2];
                            found = true;
                        }
                    }
                }

            }
            else
            {
                neighboursNull++;
            }
        }
    }
    else
    {
        deleteTriangle(tid);
      //shiftTriangleList(tid);
        coords[vid0_ptr + 0] = coords[vid1_ptr + 0];
        coords[vid0_ptr + 1] = coords[vid1_ptr + 1];
        coords[vid0_ptr + 2] = coords[vid1_ptr + 2];
    }
    if(neighboursNull == 3)
    {
        return false;
    }
    else
    {
        return true;
    }
}



/** Quando un triangolo viene eliminato per una ragione o per un'altra
 *  la lista può essere shiftata se al posto delle posizioni delle coordinate
 *  del triangolo cancellato si inserisce la costante TID_ELIM
 */

bool DrawableSkel::shiftTriangleList()
{
    for(int tid = 0; tid < tris.size()/3; tid++)
    {
        int tid_ptr  = 3 * tid;
        int vid0     = tris[tid_ptr + 0];
        int vid1     = tris[tid_ptr + 1];
        int vid2     = tris[tid_ptr + 2];

        if(vid0 == TID_ELIM && vid1 == TID_ELIM && vid2 == TID_ELIM)
        {
            if(tris[tris.size()-1] != TID_ELIM)
            {
                tris[tid_ptr+2] = tris[tris.size()-1];
                tris.pop_back();
                tris[tid_ptr+1] = tris[tris.size()-1];
                tris.pop_back();
                tris[tid_ptr+0] = tris[tris.size()-1];
                tris.pop_back();
            }
            else
            {
                 tris.pop_back();
                 tris.pop_back();
                 tris.pop_back();
                 tid--;
            }
        }
        else
        {
            if(vid0 == TID_ELIM || vid1 == TID_ELIM || vid2 == TID_ELIM)
            {
                return false;
            }
        }
    }
    return true;

}

bool DrawableSkel::shiftTriangleList(int tid)
{
    int tid_ptr  = 3 * tid;
    int vid0     = tris[tid_ptr + 0];
    int vid1     = tris[tid_ptr + 1];
    int vid2     = tris[tid_ptr + 2];
    bool delComplete = true;
    do
    {
        if(!(vid2 == tris[tris.size()-1]))
        {
            if(vid0 == TID_ELIM && vid1 == TID_ELIM && vid2 == TID_ELIM)
            {
                if(tris[tris.size()-1] != TID_ELIM)
                {
                    tris[tid_ptr+2] = tris[tris.size()-1];
                    tris.pop_back();
                    tris[tid_ptr+1] = tris[tris.size()-1];
                    tris.pop_back();
                    tris[tid_ptr+0] = tris[tris.size()-1];
                    tris.pop_back();
                }
                else
                {
                     tris.pop_back();
                     tris.pop_back();
                     tris.pop_back();
                     delComplete = false;

                }
            }
            else
            {
                if(vid0 == TID_ELIM || vid1 == TID_ELIM || vid2 == TID_ELIM)
                {
                    return false;
                }
            }
        }
        else
        {
            tris.pop_back();
            tris.pop_back();
            tris.pop_back();
        }
    }
    while(!delComplete);
    return true;
}

bool DrawableSkel::deleteTriangle(int tid)
{
    int tid_ptr  = 3 * tid;
    tris[tid_ptr + 0] = TID_ELIM;
    tris[tid_ptr + 1] = TID_ELIM;
    tris[tid_ptr + 2] = TID_ELIM;
    return true;
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

bool DrawableSkel::deleteVTX(int vid)
{
//    int tid;
//    int tid_ptr;
//    for(unsigned int i=0; vtx2tri[vid].size(); i++)
//    {
//        tid = (vtx2tri[vid])[i];
//        tid_ptr = tid*3;
//        if(tris[tid_ptr] == vid)
//    }


}
