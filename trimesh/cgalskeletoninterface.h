#ifndef CGALSKELETONINTERFACE_H
#define CGALSKELETONINTERFACE_H

#include "common/point.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/boost/graph/properties_Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>


typedef CGAL::Simple_cartesian<double>                           Kernel;
typedef Kernel::Point_3                                          CgalPoint;
typedef CGAL::Surface_mesh<CgalPoint>                            Triangle_mesh;
typedef boost::graph_traits<Triangle_mesh>::vertex_descriptor    vertex_descriptor;
typedef CGAL::Mean_curvature_flow_skeletonization<Triangle_mesh> Skeletonization;
typedef Skeletonization::Skeleton                                Skeleton;
typedef Skeleton::vertex_descriptor                              Skeleton_vertex;
typedef Skeleton::edge_descriptor                                Skeleton_edge;


namespace CgalSkeletonInterface
{
    std::list<std::pair<Pointd, Pointd>> computeSkeleton(const char* input);


}

#endif // CGALSKELETONINTERFACE_H
