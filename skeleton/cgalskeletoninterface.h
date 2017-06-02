#ifndef CGALSKELETONINTERFACE_H
#define CGALSKELETONINTERFACE_H

#include "common/point.h"
#include <QDebug>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/boost/graph/properties_Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>
#include "skel.h"
#include "trimesh/load_save_trimesh.h"


typedef CGAL::Simple_cartesian<double>                           Kernel;
typedef Kernel::Point_3                                          CgalPoint;
typedef CGAL::Polyhedron_3<Kernel>                               Polyhedron3;
typedef CGAL::Surface_mesh<CgalPoint>                            Triangle_mesh;
typedef boost::graph_traits<Triangle_mesh>::vertex_descriptor    vertex_descriptor;
typedef CGAL::Mean_curvature_flow_skeletonization<Triangle_mesh> Skeletonization;
typedef Skeletonization::Skeleton                                Skeleton;
typedef Skeleton::vertex_descriptor                              Skeleton_vertex;
typedef Skeleton::edge_descriptor                                Skeleton_edge;
typedef Skeletonization::Meso_skeleton                           Meso_skeleton;
typedef boost::graph_traits<Meso_skeleton>::vertex_descriptor    Meso_Vertex;
typedef boost::graph_traits<Meso_skeleton>::edge_descriptor      Meso_Edge;
typedef boost::graph_traits<Meso_skeleton>::edge_iterator        Meso_Iterator;

struct skelComponents {
    std::list<std::pair<Pointd, Pointd> > listaEdge;
    std::vector<int> listaTriangoli;
    std::vector<double> listaPunti;
};

namespace CgalSkeletonInterface
{

    skelComponents computeSkeleton(const char* input);

    //std::list<std::pair<Pointd, Pointd> > computeMesoSkeleton(const char* input);

}


#endif // CGALSKELETONINTERFACE_H
