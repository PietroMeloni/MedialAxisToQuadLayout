#ifndef SKELETONCREATOR_H
#define SKELETONCREATOR_H

#include "iostream"
#include "fstream"
#include "common/point.h"
#include <QDebug>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/boost/graph/properties_Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>
#include "skel.h"
#include "trimesh/load_save_trimesh.h"
#include "common/trischar.h"


//using namespace std;
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



class SkeletonCreator
{
    public:
        SkeletonCreator();
        SkeletonCreator(const char* input);
        SkeletonCreator(const char* input, int numCompressions);
        void computeSkeleton(const char* input);
        void computeSkeleton(const char* input, int numCompressions);
        void computeMesoSkeleton(const char* input);
        void compressMesoSkeleton(double trashold);


        inline std::list<std::pair<Pointd, Pointd> > getListaEdge()
        {
            return listaEdge;
        }
        inline std::vector<int> getListaTriangoli()
        {
            return listaTriangoli;
        }
        inline std::vector<double> getListaPunti()
        {
            return listaPunti;
        }

        inline double getSurfaceArea()
        {
            return surfaceArea;
        }

private:
        std::list<std::pair<Pointd, Pointd> > listaEdge;
        std::vector<int> listaTriangoli;
        std::vector<double> listaPunti;
        double surfaceArea;
        static double getTriangleArea(Pointd a, Pointd b, Pointd c);
        //ofstream log;

};


#endif // SKELETONCREATOR_H
