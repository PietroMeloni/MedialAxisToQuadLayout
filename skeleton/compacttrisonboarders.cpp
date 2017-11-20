#include "compacttrisonboarders.h"

CompactTrisOnBoarders::CompactTrisOnBoarders()
{

}


CompactTrisOnBoarders::CompactTrisOnBoarders(std::vector<int>& tris, std::vector<double>& coords, double threshold, std::vector<std::vector<int>> tri2Tri, std::vector<std::vector<int>> vtx2Tri)
{
    tri2tri = tri2Tri;
    vtx2tri = vtx2Tri;
    fillSets(tris, coords, threshold);

}

CompactTrisOnBoarders::CompactTrisOnBoarders(std::vector<int> &tris, std::vector<double> &coords, double threshold, std::vector<std::vector<int> > tri2Tri, std::vector<std::vector<int> > vtx2Tri, int numOfTris2BeDeleted)
{
    tri2tri = tri2Tri;
    vtx2tri = vtx2Tri;
    fillSetsTestFunction(tris,coords,0.0,numOfTris2BeDeleted);

}
/**
 * @brief CompactTrisOnBoarders::fillSets fills sets containing tri and vtx to be deleted
 * @param tris
 * @param coords
 * @param threshold
 */
void CompactTrisOnBoarders::fillSets(std::vector<int>& tris, std::vector<double>& coords, double threshold)
{
    bool found = false;
    int n_t_edit = 0;
    int last_tris = (tris.size()/3);
    for(int tid=0; tid < last_tris; ++tid)
    {
        int tid_ptr  = 3 * tid;
        int vid0     = tris[tid_ptr + 0];
        int vid1     = tris[tid_ptr + 1];
        int vid2     = tris[tid_ptr + 2];
        int vid0_ptr = 3 * vid0;
        int vid1_ptr = 3 * vid1;
        int vid2_ptr = 3 * vid2;

        //triangle vertexes
        Pointd a(coords[vid0_ptr], coords[vid0_ptr + 1], coords[vid0_ptr + 2]);
        Pointd b(coords[vid1_ptr], coords[vid1_ptr + 1], coords[vid1_ptr + 2]);
        Pointd c(coords[vid2_ptr], coords[vid2_ptr + 1], coords[vid2_ptr + 2]);

        //shortest edge's vertexes's id
        int vtx1, vtx2;

        //this 2 vtx are the vtx that are connected by the shortest edge.
        Pointd firstVtxOfShortestEdge;
        Pointd secondVtxOfShortestEdge;

        //if area is less then threshold and the tris is not already processed...
        if(trisCharacteristic::isAreaLessThan(a,b,c,threshold) && (std::find(alreadyCompactedTris.begin(), alreadyCompactedTris.end(), tid) == alreadyCompactedTris.end()))
        {
            //push_back tri in the set of already processed tris
            alreadyCompactedTris.push_back(tid);
            n_t_edit++;
            //this int is an indicator of whitch edge is the smaller in the current tri
            int minorEdge = trisCharacteristic::minDistanceBetweenThreePoints(a,b,c);

            //setting vtx in a way that can be used like pointd and like int
            if(minorEdge == 0)
            {
                //minor edge is ab
                firstVtxOfShortestEdge = a;
                secondVtxOfShortestEdge = b;
                vtx1 = vid0;
                vtx2 = vid1;
            }
            else
            {
                if(minorEdge == 1)
                {
                    //minor edge is ac
                    firstVtxOfShortestEdge = a;
                    secondVtxOfShortestEdge = c;
                    vtx1 = vid0;
                    vtx2 = vid2;
                }
                else
                {
                    //minor edge is bc
                    firstVtxOfShortestEdge = b;
                    secondVtxOfShortestEdge = c;
                    vtx1 = vid1;
                    vtx2 = vid2;
                }
            }
            found=false;


            for(int i = 0; i < 3 && !found; i++)
            {

                int neigh = (tri2tri[tid])[i];
                int neigh_ptr = neigh*3;
                int vid0     = tris[neigh_ptr + 0];
                int vid1     = tris[neigh_ptr + 1];
                int vid2     = tris[neigh_ptr + 2];
                int vid0_ptr = 3 * vid0;
                int vid1_ptr = 3 * vid1;
                int vid2_ptr = 3 * vid2;
                Pointd aN(coords[vid0_ptr], coords[vid0_ptr + 1], coords[vid0_ptr + 2]);
                Pointd bN(coords[vid1_ptr], coords[vid1_ptr + 1], coords[vid1_ptr + 2]);
                Pointd cN(coords[vid2_ptr], coords[vid2_ptr + 1], coords[vid2_ptr + 2]);

                //qui iniziano i problemi
                bool edgeFound = trisCharacteristic::hasTidThisTwoVertexes(firstVtxOfShortestEdge, secondVtxOfShortestEdge, aN, bN, cN);
                if(edgeFound)
                {
                    found = true;
                    //sapendo che il vicino con il lato più corto
                    //è il vicino i-esimo,  gli altri due devono essere
                    //per forza quelli che diventano vicini tra loro
                    int neighOfTId1 = (tri2tri[tid])[(i+1)%3];
                    int neighOfTId2 = (tri2tri[tid])[(i+2)%3];

                    alreadyCompactedTris.push_back(neigh);

                    int neighOfNeigh1, neighOfNeigh2;

                    //mi basta metterli al posto di tid, che verrà cancellato
                    for(int j = 0; j < 3; j++)
                    {
                        if((tri2tri[neighOfTId1])[j] == tid)
                        {
                            (tri2tri[neighOfTId1])[j] = neighOfTId2;
                        }
                        if((tri2tri[neighOfTId2])[j] == tid)
                        {
                            (tri2tri[neighOfTId2])[j] = neighOfTId1;
                        }

                    }
                    //trovo i vicini non-tid di neight
                    for(int j = 0; j < 3; j++)
                    {
                        if((tri2tri[neigh])[j] == tid)
                        {
                            neighOfNeigh1 = (tri2tri[neigh])[(j+1)%3];
                            neighOfNeigh2 = (tri2tri[neigh])[(j+2)%3];
                        }


                    }
                    //imposto i vicini non-tid di neight vicini tra loro
                    //sostituendo neight, perche' verra' cancellato
                    for(int j = 0; j < 3; j++)
                    {
                        if((tri2tri[neighOfNeigh1])[j] == neigh)
                        {
                            (tri2tri[neighOfNeigh1])[j] = neighOfNeigh2;
                        }
                        if((tri2tri[neighOfNeigh2])[j] == neigh)
                        {
                            (tri2tri[neighOfNeigh2])[j] = neighOfNeigh1;
                        }

                    }

                    buildNewCoordAdj(vtx1, vtx2, tid, neigh, tris, coords);


                }


            }


        }
    }

    deleteTrisFromList( tris );

}
/**
 * @brief CompactTrisOnBoarders::fillSetsTestFunction test function
 * @param tris
 * @param coords
 * @param threshold
 * @param numOfTris2BeDeleted
 */
void CompactTrisOnBoarders::fillSetsTestFunction(std::vector<int> &tris, std::vector<double> &coords, double threshold, int numOfTris2BeDeleted)
{
    bool found = false;
    //test variable, not usefull
    int n_t_edit = 0;

    for(int tid=0; tid < numOfTris2BeDeleted; ++tid)
    {
        int tid_ptr  = 3 * tid;
        int vid0     = tris[tid_ptr + 0];
        int vid1     = tris[tid_ptr + 1];
        int vid2     = tris[tid_ptr + 2];
        int vid0_ptr = 3 * vid0;
        int vid1_ptr = 3 * vid1;
        int vid2_ptr = 3 * vid2;

        //triangle vertexes
        Pointd a(coords[vid0_ptr], coords[vid0_ptr + 1], coords[vid0_ptr + 2]);
        Pointd b(coords[vid1_ptr], coords[vid1_ptr + 1], coords[vid1_ptr + 2]);
        Pointd c(coords[vid2_ptr], coords[vid2_ptr + 1], coords[vid2_ptr + 2]);

        //shortest edge's vertexes's id
        int vtx1, vtx2;

        //this 2 vtx are the vtx that are connected by the shortest edge.
        Pointd firstVtxOfShortestEdge;
        Pointd secondVtxOfShortestEdge;

        //push_back tri in the set of already processed tris
        alreadyCompactedTris.push_back(tid);
        n_t_edit++;
        //this int is an indicator of whitch edge is the smaller in the current tri
        int minorEdge = trisCharacteristic::minDistanceBetweenThreePoints(a,b,c);

        //setting vtx in a way that can be used like pointd and like int
        if(minorEdge == 0)
        {
            //minor edge is ab
            firstVtxOfShortestEdge = a;
            secondVtxOfShortestEdge = b;
            vtx1 = vid0;
            vtx2 = vid1;
        }
        else
        {
            if(minorEdge == 1)
            {
                //minor edge is ac
                firstVtxOfShortestEdge = a;
                secondVtxOfShortestEdge = c;
                vtx1 = vid0;
                vtx2 = vid2;
            }
            else
            {
                //minor edge is bc
                firstVtxOfShortestEdge = b;
                secondVtxOfShortestEdge = c;
                vtx1 = vid1;
                vtx2 = vid2;
            }
        }
        found=false;


        for(int i = 0; i < 3 && !found; i++)
        {

            int neigh = (tri2tri[tid])[i];
            int neigh_ptr = neigh*3;
            int vid0     = tris[neigh_ptr + 0];
            int vid1     = tris[neigh_ptr + 1];
            int vid2     = tris[neigh_ptr + 2];
            int vid0_ptr = 3 * vid0;
            int vid1_ptr = 3 * vid1;
            int vid2_ptr = 3 * vid2;
            Pointd aN(coords[vid0_ptr], coords[vid0_ptr + 1], coords[vid0_ptr + 2]);
            Pointd bN(coords[vid1_ptr], coords[vid1_ptr + 1], coords[vid1_ptr + 2]);
            Pointd cN(coords[vid2_ptr], coords[vid2_ptr + 1], coords[vid2_ptr + 2]);

            //qui iniziano i problemi
            bool edgeFound = trisCharacteristic::hasTidThisTwoVertexes(firstVtxOfShortestEdge, secondVtxOfShortestEdge, aN, bN, cN);
            if(edgeFound)
            {
                found = true;
                //sapendo che il vicino con il lato più corto
                //è il vicino i-esimo,  gli altri due devono essere
                //per forza quelli che diventano vicini tra loro
                int neighOfTId1 = (tri2tri[tid])[(i+1)%3];
                int neighOfTId2 = (tri2tri[tid])[(i+2)%3];

                alreadyCompactedTris.push_back(neigh);

                int neighOfNeigh1, neighOfNeigh2;

                //mi basta metterli al posto di tid, che verrà cancellato
                for(int j = 0; j < 3; j++)
                {
                    if((tri2tri[neighOfTId1])[j] == tid)
                    {
                        (tri2tri[neighOfTId1])[j] = neighOfTId2;
                    }
                    if((tri2tri[neighOfTId2])[j] == tid)
                    {
                        (tri2tri[neighOfTId2])[j] = neighOfTId1;
                    }

                }
                //trovo i vicini non-tid di neight
                for(int j = 0; j < 3; j++)
                {
                    if((tri2tri[neigh])[j] == tid)
                    {
                        neighOfNeigh1 = (tri2tri[neigh])[(j+1)%3];
                        neighOfNeigh2 = (tri2tri[neigh])[(j+2)%3];
                    }


                }
                //imposto i vicini non-tid di neight vicini tra loro
                //sostituendo neight, perche' verra' cancellato
                for(int j = 0; j < 3; j++)
                {
                    if((tri2tri[neighOfNeigh1])[j] == neigh)
                    {
                        (tri2tri[neighOfNeigh1])[j] = neighOfNeigh2;
                    }
                    if((tri2tri[neighOfNeigh2])[j] == neigh)
                    {
                        (tri2tri[neighOfNeigh2])[j] = neighOfNeigh1;
                    }

                }

                buildNewCoordAdj(vtx1, vtx2, tid, neigh, tris, coords);


            }


        }



    }

    deleteTrisFromList( tris );
}


/**
 * @brief CompactTrisOnBoarders::buildNewCoordAdj mantains congruent list of cords and neigh of vtx
 * @param firstVTX
 * @param secondVTX
 * @param tid
 * @param neigh
 * @param tris
 * @param coords
 */
void CompactTrisOnBoarders::buildNewCoordAdj(int firstVTX, int secondVTX, int tid, int neigh, std::vector<int>& tris, std::vector<double>& coords)
{
    //assert(firstVTX != secondVTX);
    int num_changes_loop = 0;

    //for every triangle that has as vtx firstVTX, change it with secondVTX
    for(int i=0; i < vtx2tri[firstVTX].size(); i++)
    {
        int triN = (vtx2tri[firstVTX])[i];
        int triN_ptr = triN*3;
        if(triN != tid && triN != neigh)
        {
            for(int j=0; j < 3; j++)
            {
                //assert(tris[triN_ptr+j] != secondVTX);
                if(tris[triN_ptr+j] == firstVTX)
                {
                    tris[triN_ptr+j] = secondVTX;
                    num_changes_loop++;

                }
            }
        }
        //check for errors because in many times there are
        //triangles with 2 equal vtx that are not to be deleted
        if(triN != tid && triN != neigh && (tris[triN_ptr] == tris[triN_ptr+1] ||
           (tris[triN_ptr+1] == tris[triN_ptr+2]) || (tris[triN_ptr] == tris[triN_ptr+2]))&&
            std::find(alreadyCompactedTris.begin(), alreadyCompactedTris.end(), triN) == alreadyCompactedTris.end() )
        {
            alreadyCompactedTris.sort();
            qDebug() << "triN = " <<triN << "has 2 or more vtx equal";
        }
    }

    if(num_changes_loop < 2)
    {
        qDebug()<< "num changes inside loop < 2. For tid = "<<tid<<";";
    }
   //update list of vtx2tri removing the 2 triangles that will be deleted.
   for(int i = 0; i < 3; i++)
   {
        int tid_ptr = tid*3;
        int neigh_ptr = neigh*3;
        int vtxT = tris[tid_ptr+i];
        int vtxN = tris[neigh_ptr+i];
        //delete for value and not for position
        (vtx2tri[vtxT]).erase(std::remove((vtx2tri[vtxT]).begin(), (vtx2tri[vtxT]).end(), tid), (vtx2tri[vtxT]).end());
        (vtx2tri[vtxN]).erase(std::remove((vtx2tri[vtxN]).begin(), (vtx2tri[vtxN]).end(), neigh), (vtx2tri[vtxN]).end());

   }
    //add triangles in vtx2tri of secondVTX the ones that are contained in vtx3tri of firstVTX
   for(int i = 0; i < vtx2tri[firstVTX].size(); i++)
   {
        if(std::find(vtx2tri[secondVTX].begin(), vtx2tri[secondVTX].end(), vtx2tri[firstVTX][i]) == vtx2tri[secondVTX].end() )
        {
            vtx2tri[secondVTX].push_back(vtx2tri[firstVTX][i]);
        }
   }
   alreadyDeleteCoord.push_back(firstVTX);
}
/**
 * @brief CompactTrisOnBoarders::deleteTrisFromList delete triangles from tris.
 * @param tris
 */
void CompactTrisOnBoarders::deleteTrisFromList(std::vector<int> &tris)
{
    alreadyCompactedTris.sort();
    int i = 0;
    BOOST_FOREACH( int tid, alreadyCompactedTris )
    {
        if(!(tid >= tris.size()/3))
        {
            tid = tid - i;
            //delete for position and not for value
            tris.erase(tris.begin()+tid*3, tris.begin()+tid*3+3);
            i++;
        }
        /*tris.erase();
        tris.erase(std::remove(tris.begin(), tris.end(), tid*3), tris.end());*/

    }

    //just to make the final mesh manifold, is not a final solution
    int a,b,c;
    for(int i = 0; i < tris.size(); i+=3)
    {
        a = tris[i];
        b = tris[i+1];
        c = tris[i+2];
        if(a == b || a == c || b == c)
        {
            tris.erase(tris.begin()+i, tris.begin()+i+3);
            i-=3;
        }
    }
}






