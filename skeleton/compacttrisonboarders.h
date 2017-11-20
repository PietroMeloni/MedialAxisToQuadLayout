#ifndef COMPACTTRISONBOARDERS_H
#define COMPACTTRISONBOARDERS_H

#include "iostream"
#include <set>
#include <common/point.h>
#include <common/trischar.h>
#include <boost/foreach.hpp>
#include <QDebug>

class CompactTrisOnBoarders
{

public:

    CompactTrisOnBoarders();
    CompactTrisOnBoarders(std::vector<int>& tris, std::vector<double>& coords, double threshold, std::vector<std::vector<int>> tri2Tri, std::vector<std::vector<int>> vtx2Tri);
    CompactTrisOnBoarders(std::vector<int>& tris, std::vector<double>& coords, double threshold, std::vector<std::vector<int>> tri2Tri, std::vector<std::vector<int>> vtx2Tri, int numOfTris2BeDeleted);



private:

    std::vector<std::vector<int>> tri2tri;
    std::vector<std::vector<int>> vtx2tri;
    std::list<int> alreadyCompactedTris;
    std::list<int> alreadyDeleteCoord;

    void fillSets(std::vector<int>& tris, std::vector<double>& coords, double threshold);
    //test function, barely equal of the above one
    void fillSetsTestFunction(std::vector<int>& tris, std::vector<double>& coords, double threshold, int numOfTris2BeDeleted);
    void buildNewCoordAdj(int firstVTX, int SecondVTX, int tid, int neigh, std::vector<int>& tris, std::vector<double>& coords);
    void deleteTrisFromList(std::vector<int>& tris);
};

#endif // COMPACTTRISONBOARDERS_H
