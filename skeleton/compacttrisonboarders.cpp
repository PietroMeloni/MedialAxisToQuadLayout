#include "compacttrisonboarders.h"

CompactTrisOnBoarders::CompactTrisOnBoarders()
{

}


CompactTrisOnBoarders::CompactTrisOnBoarders(std::vector<int>& tris, std::vector<int>& coords, int threshold, std::vector<std::vector<int>> tri2Tri, std::vector<std::vector<int>> vtx2Tri)
{
    tri2tri = tri2Tri;
    vtx2tri = vtx2Tri;
    fillSets(tris, coords, threshold);

}

void CompactTrisOnBoarders::fillSets(std::vector<int>& tris, std::vector<int>& coords, int threshold)
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
        Pointd a(vid0_ptr, vid0_ptr + 1, vid0_ptr + 2);
        Pointd b(vid1_ptr, vid1_ptr + 1, vid1_ptr + 2);
        Pointd c(vid2_ptr, vid2_ptr + 1, vid2_ptr + 2);

        if(trisCharacteristic::isAreaLessThan(a,b,c,threshold))
        {
            alreadyCompactedTris.insert(tid);
        }
    }
}
