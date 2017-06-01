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
        Skel(const DrawableTrimesh &tm);


    protected:
        std::vector<Pointd> vtx_list;
        std::vector<std::pair<Pointd, Pointd>> edge_list;
};

#endif // SKEL_H
