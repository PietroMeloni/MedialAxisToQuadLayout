#ifndef SKEL_H
#define SKEL_H

#include "common/point.h"
#include "trimesh/gui/drawable_trimesh.h"

#include <fstream>

#include "cgalskeletoninterface.h"

class Skel
{
    public:
        Skel();
        Skel(std::vector<double> coord, std::vector<int> triFacets);

        void setEdges(std::vector<std::pair<Pointd, Pointd>> edges);

        std::vector<double> getVtxList();
        std::vector<int>    triangleList();

    protected:
        std::vector<double> vtx_list;
        std::vector<std::pair<Pointd, Pointd>> edge_list;
        std::vector<int> tri_list;
};

#endif // SKEL_H
