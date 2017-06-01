#include "skel.h"
Skel::Skel()
{

}

Skel::Skel(const DrawableTrimesh &tm)
{
      //std::ifstream input(tm.fileName);
      std::list<std::pair<Pointd, Pointd>> skeleton = CgalSkeletonInterface::computeSkeleton(tm.fileName);
}
