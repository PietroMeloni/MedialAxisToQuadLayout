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
/**
 * @brief CompactTrisOnBoarders::fillSets
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
        Pointd a(coords[vid0_ptr], coords[vid0_ptr + 1], coords[vid0_ptr + 2]);
        Pointd b(coords[vid1_ptr], coords[vid1_ptr + 1], coords[vid1_ptr + 2]);
        Pointd c(coords[vid2_ptr], coords[vid2_ptr + 1], coords[vid2_ptr + 2]);

        int vtx1, vtx2;

        //this 2 vtx are the vtx that are connected by the shortest edge.
        Pointd firstVtxSE;
        Pointd secondVTXSE;

        //if area is less then threshold and the tris is not already processed...
        if(trisCharacteristic::isAreaLessThan(a,b,c,threshold) && (std::find(alreadyCompactedTris.begin(), alreadyCompactedTris.end(), tid) == alreadyCompactedTris.end()))
        {
            //push_back tri in the set of already processed tris
            alreadyCompactedTris.push_back(tid);
            n_t_edit++;
            //this int is an indicator of whitch edge is the smaller in the current tri
            int minorEdge = trisCharacteristic::minDistanceBetweenThreePoints(a,b,c);

            if(minorEdge == 0)
            {
                firstVtxSE = a;
                secondVTXSE = b;
                vtx1 = vid0;
                vtx2 = vid1;
            }
            else
            {
                if(minorEdge == 1)
                {
                    firstVtxSE = a;
                    secondVTXSE = c;
                    vtx1 = vid0;
                    vtx2 = vid2;
                }
                else
                {
                    firstVtxSE = b;
                    secondVTXSE = c;
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
                bool edgeFound = trisCharacteristic::hasTidThisTwoVertexes(firstVtxSE, secondVTXSE, aN, bN, cN);
                if(edgeFound)
                {
                    found = true;
                    //sapendo che il vicino con il lato più corto
                    //è il vicino i-esimo,  gli altri due devono essere
                    //per forza quelli che divantano vicini tra loro
                    int neighT1 = (tri2tri[tid])[(i+1)%3];
                    int neighT2 = (tri2tri[tid])[(i+2)%3];

                    alreadyCompactedTris.push_back(neigh);

                    int neighN1, neighN2;

                    //mi basta metterli al posto di tid, che verrà cancellato
                    for(int j = 0; j < 3; j++)
                    {
                        if((tri2tri[neighT1])[j] == tid)
                        {
                            (tri2tri[neighT1])[j] = neighT2;
                        }
                        if((tri2tri[neighT2])[j] == tid)
                        {
                            (tri2tri[neighT2])[j] = neighT1;
                        }

                    }
                    //trovo i vicini non-tid di neight
                    for(int j = 0; j < 3; j++)
                    {
                        if((tri2tri[neigh])[j] == tid)
                        {
                            neighN1 = (tri2tri[neigh])[(j+1)%3];
                            neighN2 = (tri2tri[neigh])[(j+2)%3];
                        }


                    }
                    //imposto i vicini di neight vicini tra loro
                    //sostituendo neight, perche' verra' cancellato
                    for(int j = 0; j < 3; j++)
                    {
                        if((tri2tri[neighN1])[j] == neigh)
                        {
                            (tri2tri[neighN1])[j] = neighN2;
                        }
                        if((tri2tri[neighN2])[j] == neigh)
                        {
                            (tri2tri[neighN2])[j] = neighN1;
                        }

                    }

                    buildNewCoordAdj(vtx1, vtx2, tid, neigh, tris, coords);


                }


            }


        }
    }

    deleteTrisFromList( tris );

}

void CompactTrisOnBoarders::buildNewCoordAdj(int firstVTX, int secondVTX, int tid, int neigh, std::vector<int>& tris, std::vector<double>& coords)
{
    int num_changes_loop = 0;
    for(int i=0; i < vtx2tri[firstVTX].size(); i++)
    {
        int triN = (vtx2tri[firstVTX])[i];
        int triN_ptr = triN*3;

        for(int j=0; j < 3; j++)
        {
            if(tris[triN_ptr+j] == firstVTX)
            {
                tris[triN_ptr+j] = secondVTX;
                num_changes_loop++;

            }
        }
    }

    if(num_changes_loop < 2)
    {
        qDebug()<< "num changes inside loop < 2. For tid = "<<tid<<";";
    }

   for(int i = 0; i < 3; i++)
   {
        int tid_ptr = tid*3;
        int neigh_ptr = neigh*3;
        int vtxT = tris[tid_ptr+i];
        int vtxN = tris[neigh_ptr+i];
        (vtx2tri[vtxT]).erase(std::remove((vtx2tri[vtxT]).begin(), (vtx2tri[vtxT]).end(), tid), (vtx2tri[vtxT]).end());
        (vtx2tri[vtxN]).erase(std::remove((vtx2tri[vtxN]).begin(), (vtx2tri[vtxN]).end(), neigh), (vtx2tri[vtxN]).end());

   }

   for(int i = 0; i < vtx2tri[firstVTX].size(); i++)
   {
        if(std::find(vtx2tri[secondVTX].begin(), vtx2tri[secondVTX].end(), vtx2tri[firstVTX][i]) == vtx2tri[secondVTX].end() )
        {
            vtx2tri[secondVTX].push_back(vtx2tri[firstVTX][i]);
        }
   }
   alreadyDeleteCoord.push_back(firstVTX);
}

void CompactTrisOnBoarders::deleteTrisFromList(std::vector<int> &tris)
{
    alreadyCompactedTris.sort();
    int i = 0;
    BOOST_FOREACH( int tid, alreadyCompactedTris )
    {
        if(!(tid >= tris.size()/3))
        {
            tid = tid - i;
            tris.erase(tris.begin()+tid*3, tris.begin()+tid*3+3);
            i++;
        }
        /*tris.erase();
        tris.erase(std::remove(tris.begin(), tris.end(), tid*3), tris.end());*/

    }
}






