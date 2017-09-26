#ifndef COMPACTTRISONBOARDERS_H
#define COMPACTTRISONBOARDERS_H

#include "iostream"
#include <set>
#include <common/point.h>
#include <common/trischar.h>
class CompactTrisOnBoarders
{

public:

    CompactTrisOnBoarders();
    CompactTrisOnBoarders(std::vector<int>& tris, std::vector<int>& coords, int threshold, std::vector<std::vector<int>> tri2Tri, std::vector<std::vector<int>> vtx2Tri);

private:

    std::vector<std::vector<int>> tri2tri;
    std::vector<std::vector<int>> vtx2tri;
    std::set<int> alreadyCompactedTris;
    std::set<int> alreadyDeleteCoord;

    void fillSets(std::vector<int>& tris, std::vector<int>& coords, int threshold);
};

#endif // COMPACTTRISONBOARDERS_H
