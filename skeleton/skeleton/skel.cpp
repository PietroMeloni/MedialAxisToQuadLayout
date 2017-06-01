#include "skel.h"
Skel::Skel()
{

}

Skel::Skel(std::vector<double> coord, std::vector<int> triFacets)
{
    this->vtx_list = coord;
    this->tri_list = triFacets;
}

void Skel::setEdges(std::vector<std::pair<Pointd, Pointd> > edges)
{
    this->edge_list = edges;
}

std::vector<double> Skel::getVtxList()
{
    return this->vtx_list;
}

std::vector<int> Skel::triangleList()
{
    return this->tri_list;
}
