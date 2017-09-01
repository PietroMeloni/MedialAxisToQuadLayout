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

    logPietro.open("/home/pietro/Desktop/log.txt");
    logPietro << "Entering Constructor: \n\n";
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
            logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
            logPietro << "tid = " + std::to_string(tid)+ " is on border\n";
            logPietro.close();
            mergeTwoVertexes(tid);
            shiftTriangleList();
            //buildAdjacency();
            tid--;
            logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
            logPietro << "\n\n\n";
            logPietro.close();

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

    int minDist = minDistanceBetweenThreePoints(vA, vB, vC);
    int neighboursNull = 0;
    bool found = false;

    //controllo se tri2tri è una lista corretta prima di andare avanti.
    //Se un triangolo ha più di 3 o meno di 3 vicini c'è qualcosa che non va
    if(tri2tri[tid].size() == 3)
    {

        for(int i=0; i < tri2tri[tid].size()&&!found; i++)
        {
            int tid_ptr0  = 3 * (tri2tri[tid])[i];
            if( tid_ptr0 >= 0)
            {
             //faccio il merge dei punti che compongono l'edge con la distanza minima.
             //Se minDist == 0, l'edge più corto sta nei primi due vertici
               if(minDist == 0)
               {
                    //devo trovare il triangolo vicino di tid che abbia quei due vertici
                    //una volta trovato so quali triangoli eliminare
                    if(hasTidThisTwoVertexes(tid_ptr0/3, vA, vB))
                    {

                        logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
                        logPietro << std::to_string((tri2tri[tid])[i])+" will be deleted with tid="+std::to_string(tid)+"\n";
                        logPietro.close();
                        deleteVTX(vid0, vid1);
                        updateDeletedTriNeighbours(tid, (tri2tri[tid])[i]);
                        deleteTriangle((tri2tri[tid])[i]);
                        deleteTriangle(tid);

                        found = true;
                    }
               }
               else
               {
                   //primo e terzo
                   if(minDist == 1)
                   {
                       if(hasTidThisTwoVertexes(tid_ptr0/3, vA, vC))
                       {
                           logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
                           logPietro << std::to_string((tri2tri[tid])[i])+" will be deleted with tid="+std::to_string(tid)+"\n";
                           logPietro.close();
                           deleteVTX(vid0, vid2);
                           updateDeletedTriNeighbours(tid, (tri2tri[tid])[i]);
                           deleteTriangle((tri2tri[tid])[i]);
                           deleteTriangle(tid);

                           found = true;
                       }
                   }
                   else
                   {
                       //secondo e terzo
                       if(hasTidThisTwoVertexes(tid_ptr0/3, vB, vC))
                       {
                           logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
                           logPietro << std::to_string((tri2tri[tid])[i])+" will be deleted with tid="+std::to_string(tid)+"\n";
                           logPietro.close();
                           deleteVTX(vid2, vid1);
                           updateDeletedTriNeighbours(tid, ((tri2tri[tid])[i]));
                           deleteTriangle((tri2tri[tid])[i]);
                           deleteTriangle(tid);

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
        return false;
    }
    if(neighboursNull == 3)
    {
        return false;
    }
    else
    {
        return found;
    }
}




/**
 * @brief DrawableSkel::shiftTriangleList shifta la lista di triangoli in modo da non lasciare buchi quando
 *        alcuni triangoli vengono cancellati. Per fare questo, sposta l'ultimo triangolo nel buco e aggiorna
 *        tutte le liste in modo che non ci siano problemi.
 * @return true se tutto va bene, false altrimenti.
 */
bool DrawableSkel::shiftTriangleList()
{
    logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
    //non c'è bisogno di controllare gli indici. Il ciclo si ferma se crescono troppo
    for(int tid = 0; tid < tris.size()/3; tid++)
    {
        int tid_ptr  = 3 * tid;
        int vid0     = tris[tid_ptr + 0];
        int vid1     = tris[tid_ptr + 1];
        int vid2     = tris[tid_ptr + 2];
        if(vid0 == TID_ELIM && vid1 == TID_ELIM && vid2 == TID_ELIM)
        {
            //se non sto cancellando l'ultimo triangolo...
            if(tid_ptr + 2 != tris.size()-1)
            {
                logPietro << "tid =" + std::to_string(tid) + " will be replaced in this way in tris:\n";

                tris[tid_ptr+2] = tris[tris.size()-1];

                logPietro << "tris[" + std::to_string(tid_ptr +2)+"] = " + std::to_string(tris[tris.size()-1]) + "\n";

                tris.pop_back();
                tris[tid_ptr+1] = tris[tris.size()-1];

                logPietro << "tris[" + std::to_string(tid_ptr +1)+"] = " + std::to_string(tris[tris.size()-1]) + "\n";

                tris.pop_back();

                logPietro << "tris[" + std::to_string(tid_ptr)+"] = " + std::to_string(tris[tris.size()-1]) + "\n";

                tris[tid_ptr+0] = tris[tris.size()-1];
                tris.pop_back();

                logPietro << " last 3 istances of tris deleted\n";

                //sto spostando l'ultimo triangolo della lista al posto di quello eliminato.
                //quindi devo aggiornare la lista dei suoi vicini
                (tri2tri[tid]).clear();

                logPietro << "prev list of neigh of tid cleared.\n";
                for(int i=0; i < 3; ++i)
                {
                    logPietro << "Inserting " + std::to_string((tri2tri[tri2tri.size()-1])[i]) + " in the new neigh list\n";
                    int neighbour =(tri2tri[tri2tri.size()-1])[i];
                    (tri2tri[tid]).push_back(neighbour);
                    //aggiorno i triangoli vicini di quello che è stato spostato perché verrà eliminato.
                    logPietro << "In neigh = " + std::to_string(neighbour) + ", replace last tri = " + std::to_string(tri2tri.size()-1) + "with tid? \n";
                    for(int j=0; j < 3; ++j)
                    {
                        if((tri2tri[neighbour])[j] == tri2tri.size()-1)
                        {
                            logPietro << "Yes!\n";
                            (tri2tri[neighbour])[j] = tid;
                        }
                    }
                }
                logPietro << "tri2tri.pop_back(). Removed " + std::to_string(tri2tri.size()-1) +"\n";

                tri2tri.pop_back();
            }
            else
            {
                 //non devo aggiornare la lista dei vicini perché,
                //in caso sia da togliere l'ultimo triangolo della lista,
                //la lista degli altri vicini dovrebbe già essere aggiornata
                //dalla funzione updateDeltriNeighbours
                logPietro << "Tris removed: " + std::to_string(tris[tris.size()-1])+ "\n";

                 tris.pop_back();
                 logPietro << "Tris removed: "+ std::to_string(tris[tris.size()-1])+ "\n";
                 tris.pop_back();
                 logPietro << "Tris removed: " + std::to_string(tris[tris.size()-1])+ "\n";
                 tris.pop_back();
                 logPietro << "Tris removed also from tri2tri: tri2tri.pop_back()\n";
                 tri2tri.pop_back();
                 tid--;
            }
        }
        else
        {

            //qualcosa è stato settato male, non possibile che meno di 3 vertici
            //siano settati a TID_ELIM
            if(vid0 == TID_ELIM || vid1 == TID_ELIM || vid2 == TID_ELIM)
            {
                logPietro << "Strange exception. Less than 3 instances set to TID_ELIM\n";
                logPietro.close();
                return false;
            }
        }
    }
    logPietro.close();
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
    logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
    logPietro << std::to_string(tid)+" set to TID_ELIM in tris\n";
    logPietro.close();
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

/**
 * @brief DrawableSkel::deleteVTX cancella un vertice dalla lista
 *        e la lascia congrua con tris
 * @param vid vertice che viene cancellato
 * @param vid0 vertice che viene sostituito a quello cancellato
 * @return true se l'operazione è andata a buon fine, false altrimenti
 */
bool DrawableSkel::deleteVTX(int vid, int vid0)
{
    int tid;
    int tid_ptr;
    int lastVTX = numVertices()-1;
    logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
    logPietro << "Now vid="+ std::to_string(vid) + " will be merged with vid0= "+std::to_string(vid0)+". vid0 will replace vid\n";
    //se il vertice che sto cercando di mergiare è l'ultimo della lista,
    //devo preoccuparmi solo dei suoi vicini
    if (vid == lastVTX)
    {
        logPietro << "Since vid="+ std::to_string(vid) + "is the last VTX in the list:\n";

        bool found = false;
        int j = 0;
        for(int i=0; i < (vtx2tri[vid]).size(); i++)
        {
            tid = (vtx2tri[vid])[i];
            tid_ptr = tid*3;
            found = false;
            //scorro i vertici alla ricerca di quello da sostituire
            for(j = 0; (j < 3)&&(!found); j++)
            {

                if(tris[tid_ptr+j] == vid)
                {
                    logPietro << "tris["+ std::to_string(tid_ptr+j) + "]= "+std::to_string(tris[tid_ptr+j])+" set to vid0\n";

                    tris[tid_ptr+j] = vid0;

                }
            }
            //se non è già presente aggiungo vtx2tri[vid])[i]
            if(std::find(vtx2tri[vid0].begin(), vtx2tri[vid0].end(), (vtx2tri[vid])[i]) == vtx2tri[vid0].end())
            {
                logPietro << "vtx2tri["+ std::to_string(vid0) + "].push_back("+std::to_string((vtx2tri[vid])[i])+")\n";
                vtx2tri[vid0].push_back((vtx2tri[vid])[i]);
            }
        }
        logPietro << "Removing by pop_back"+std::to_string(coords[coords.size()-1])+" from coords\n";

        coords.pop_back();
        logPietro << "Removing by pop_back"+std::to_string(coords[coords.size()-1])+" from coords\n";

        coords.pop_back();
        logPietro << "Removing by pop_back"+std::to_string(coords[coords.size()-1])+" from coords\n";

        coords.pop_back();
        logPietro << "vtx2tri.pop_back()\n";

        vtx2tri.pop_back();

    }
    else
    {
        logPietro << "Since vid="+ std::to_string(vid) + "is not the last VTX in the list:\n";
        bool found = false;
        int j = 0;
        for(int i=0; i < (vtx2tri[vid]).size(); i++)
        {
            tid = (vtx2tri[vid])[i];
            tid_ptr = tid*3;
            found = false;

            //cerco tra i vertici quello da sostituire
            for(j = 0; (j < 3)&&(!found); j++)
            {

                if(tris[tid_ptr+j] == vid)
                {
                    logPietro << "tris["+ std::to_string(tid_ptr+j) + "]= "+std::to_string(tris[tid_ptr+j])+" set to vid0\n";

                    tris[tid_ptr+j] = vid0;

                    found = true;

                }
            }
            //se nella lista dei triangoli vicini di vid0 alcuni della lista di vid non sono presenti, li aggiungo
            if(std::find((vtx2tri[vid0]).begin(), (vtx2tri[vid0]).end(), (vtx2tri[vid])[i]) == vtx2tri[vid0].end())
            {
                logPietro << "vtx2tri["+ std::to_string(vid0) + "].push_back("+std::to_string((vtx2tri[vid])[i])+")\n";
                vtx2tri[vid0].push_back((vtx2tri[vid])[i]);
            }

        }
        //per non lasciare un "buco" nella lista, sostituisco l'ultimo vertice con vid e poi cancello l'ultimo
        //ovviamente, vanno aggiornati tutti i triangoli che avessero a che fare con l'ultimo vertice
        (vtx2tri[vid]).clear();
        logPietro << "LastVTX == "+std::to_string(lastVTX)+" will replace vid. vtx2tri[vid] cleared.\n";

        for(int i=0; i < vtx2tri[lastVTX].size(); i++)
        {
            tid = (vtx2tri[lastVTX])[i];
            tid_ptr = tid*3;
            for(int j = 0; j < 3; j++)
            {
                if(tris[tid_ptr+j] == lastVTX)
                {
                    logPietro << "tris["+ std::to_string(tid_ptr+j) + "] == "+std::to_string(tris[tid_ptr+j])+" = vid\n";
                    tris[tid_ptr+j] = vid;

                }
            }
            logPietro << "vtx2tri["+ std::to_string(vid) + "].push_back( "+std::to_string((vtx2tri[lastVTX])[i])+")\n";
            (vtx2tri[vid]).push_back((vtx2tri[lastVTX])[i]);
        }
        logPietro << "coords["+ std::to_string(vid * 3 + 2) + "] == "+std::to_string(coords[vid * 3 + 2])+" = coords[" +std::to_string(coords.size()-1)+
                "] == " +std::to_string(coords[coords.size()-1])+". coords.pop_back()\n";

        coords[vid * 3 + 2] = coords[coords.size()-1];
        coords.pop_back();
        logPietro << "coords["+ std::to_string(vid * 3 + 1) + "] == "+std::to_string(coords[vid * 3 + 2])+" = coords[" +std::to_string(coords.size()-1)+
                "] == " +std::to_string(coords[coords.size()-1])+". coords.pop_back()\n";
        coords[vid * 3 + 1] = coords[coords.size()-1];
        coords.pop_back();
        logPietro << "coords["+ std::to_string(vid * 3) + "] == "+std::to_string(coords[vid * 3])+" = coords[" +std::to_string(coords.size()-1)+
                "] == " +std::to_string(coords[coords.size()-1])+". coords.pop_back()\n";
        coords[vid * 3 + 0] = coords[coords.size()-1];
        coords.pop_back();
        logPietro << "vtx2tri.pop_back()\n";
        vtx2tri.pop_back();

    }
    logPietro.close();
    return true;


}
/**
 * @brief DrawableSkel::updateDeletedTriNeighbours aggiorno tri2tri in modo che dopo aver
 *        cancellato un triangolo, la lista dei vicini dei suoi vicini sia congrua.
 * @param tid triangolo la cui lista dei vicini verrà aggiornata, (verranno resi vicini i due
 *            vicini che non siano NeighTid
 * @param NeighTid triangolo che verrà cancellato insieme a tid.
 *        Aggiorniamo anche la sua lista, visto che ci siamo.
 * @param delVid vertice cancellato. Deve essere aggiornata almeno la lista vtx2tri.
 * @return true se tutto è andato bene false altrimenti
 */
bool DrawableSkel::updateDeletedTriNeighbours(int tid, int NeighTid)
{
    int neighbour;
    int neighbour2;
    bool found = false;

    logPietro.open("/home/pietro/Desktop/log.txt", std::fstream::app);
    logPietro << "Now neighs of tid="+ std::to_string(tid) +
           " and neighTID= "+std::to_string(NeighTid)+" will be updated\n";


    /*
     * Aggiorno i vicini di tid
     */
    //Ipotizzo: neighbour primo vicino che non sia NeighTid di tid;
    //          neighbour2 secondo vicino che non sia NeighTid;
    neighbour = (tri2tri[tid])[0];
    neighbour2 = (tri2tri[tid])[1];
    logPietro << "first neighbours of tid\n";
    if(neighbour != NeighTid)
    {
        if(neighbour2 != NeighTid)
        {
            //se entrambe le ipotesi son corrette,
            //associo tra loro i due vicini di tid (inserisco al posto di tid)
            for(int i=0; i < (tri2tri[neighbour]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour])[i] == tid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour) + "][i] == " +
                           std::to_string((tri2tri[neighbour])[i]) +
                           " = "+std::to_string(neighbour2)+"\n";
                    (tri2tri[neighbour])[i] = neighbour2;


                }
            }
            found = false;
            for(int i=0; i < (tri2tri[neighbour2]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour2])[i] == tid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour2) + "][i] == " +
                           std::to_string((tri2tri[neighbour2])[i]) +
                           " = "+std::to_string(neighbour)+"\n";
                    (tri2tri[neighbour2])[i] = neighbour;
                }
            }
        }
        //Se l'ipotesi sul secondo vicino è sbagliata,
        //quella sul primo vicino è per forza giusta
        else
        {
            neighbour2 = (tri2tri[tid])[2];
            for(int i=0; i < (tri2tri[neighbour]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour])[i] == tid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour) + "][i] == " +
                           std::to_string((tri2tri[neighbour])[i]) +
                           " = "+std::to_string(neighbour2)+"\n";
                    (tri2tri[neighbour])[i] = neighbour2;
                }
            }
            found = false;
            for(int i=0; i < (tri2tri[neighbour2]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour2])[i] == tid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour2) + "][i] == " +
                           std::to_string((tri2tri[neighbour2])[i]) +
                           " = "+std::to_string(neighbour)+"\n";
                    (tri2tri[neighbour2])[i] = neighbour;
                }
            }

        }

    }
    //se l'ipotesi sul primo vicino è falsa, allora quella sul secondo
    //dev'essere corretta
    else
    {
        neighbour = (tri2tri[tid])[2];
        for(int i=0; i < (tri2tri[neighbour]).size()&&!found; ++i)
        {
            if((tri2tri[neighbour])[i] == tid)
            {
                found = true;
                logPietro << "tri2tri["+ std::to_string(neighbour) + "][i] == " +
                       std::to_string((tri2tri[neighbour])[i]) +
                       " = "+std::to_string(neighbour2)+"\n";
                (tri2tri[neighbour])[i] = neighbour2;
            }
        }
        found = false;
        for(int i=0; i < (tri2tri[neighbour2]).size()&&!found; ++i)
        {
            if((tri2tri[neighbour2])[i] == tid)
            {
                found = true;
                logPietro << "tri2tri["+ std::to_string(neighbour2) + "][i] == " +
                       std::to_string((tri2tri[neighbour2])[i]) +
                       " = "+std::to_string(neighbour)+"\n";
                (tri2tri[neighbour2])[i] = neighbour;
            }
        }
    }
    /*
     * Aggiorno i vicini di NeighTid
     */
    //funziona allo stesso modo della precedente
    found = false;
    neighbour = (tri2tri[NeighTid])[0];
    neighbour2 = (tri2tri[NeighTid])[1];
    logPietro << "now neighbours of NeighTid\n";
    if(neighbour != tid)
    {
        if(neighbour2 != tid)
        {
            for(int i=0; i < (tri2tri[neighbour]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour])[i] == NeighTid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour) + "][i] == " +
                           std::to_string((tri2tri[neighbour])[i]) +
                           " = "+std::to_string(neighbour2)+"\n";
                    (tri2tri[neighbour])[i] = neighbour2;
                }
            }
            found = false;
            for(int i=0; i < (tri2tri[neighbour2]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour2])[i] == NeighTid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour2) + "][i] == " +
                           std::to_string((tri2tri[neighbour2])[i]) +
                           " = "+std::to_string(neighbour)+"\n";
                    (tri2tri[neighbour2])[i] = neighbour;
                }
            }
        }
        else
        {
            neighbour2 = (tri2tri[NeighTid])[2];
            for(int i=0; i < (tri2tri[neighbour]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour])[i] == NeighTid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour) + "][i] == " +
                           std::to_string((tri2tri[neighbour])[i]) +
                           " = "+std::to_string(neighbour2)+"\n";
                    (tri2tri[neighbour])[i] = neighbour2;
                }
            }
            found = false;
            for(int i=0; i < (tri2tri[neighbour2]).size()&&!found; ++i)
            {
                if((tri2tri[neighbour2])[i] == NeighTid)
                {
                    found = true;
                    logPietro << "tri2tri["+ std::to_string(neighbour2) + "][i] == " +
                           std::to_string((tri2tri[neighbour2])[i]) +
                           " = "+std::to_string(neighbour)+"\n";
                    (tri2tri[neighbour2])[i] = neighbour;
                }
            }

        }

    }
    else
    {
        neighbour = (tri2tri[tid])[2];
        for(int i=0; i < (tri2tri[neighbour]).size()&&!found; ++i)
        {
            if((tri2tri[neighbour])[i] == NeighTid)
            {
                found = true;
                logPietro << "tri2tri["+ std::to_string(neighbour) + "][i] == " +
                       std::to_string((tri2tri[neighbour])[i]) +
                       " = "+std::to_string(neighbour2)+"\n";
                (tri2tri[neighbour])[i] = neighbour2;
            }
        }
        found = false;
        for(int i=0; i < (tri2tri[neighbour2]).size()&&!found; ++i)
        {
            if((tri2tri[neighbour2])[i] == NeighTid)
            {
                found = true;
                logPietro << "tri2tri["+ std::to_string(neighbour2) + "][i] == " +
                       std::to_string((tri2tri[neighbour2])[i]) +
                       " = "+std::to_string(neighbour)+"\n";
                (tri2tri[neighbour2])[i] = neighbour;
            }
        }
    }

logPietro.close();
}
