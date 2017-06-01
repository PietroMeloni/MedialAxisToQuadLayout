#include "cgalskeletoninterface.h"

std::list<std::pair<Pointd, Pointd>> CgalSkeletonInterface::computeSkeleton(const char* path)
{
    std::ifstream input(path);
    std::pair<Pointd, Pointd> lato;
    std::list<std::pair<Pointd, Pointd>> listaLati;
    Triangle_mesh tmesh;
    input >> tmesh;
    Skeleton skeleton;
    Skeletonization mcs(tmesh);
    // 1. Contract the mesh by mean curvature flow.
    mcs.contract_geometry();
    // 2. Collapse short edges and split bad triangles.
    mcs.collapse_edges();
    mcs.split_faces();
    // 3. Fix degenerate vertices.
    mcs.detect_degeneracies();
    // Perform the above three steps in one iteration.
    mcs.contract();
    // Iteratively apply step 1 to 3 until convergence.
    mcs.contract_until_convergence();
    // Convert the contracted mesh into a curve skeleton and
    // get the correspondent surface points
    mcs.convert_to_skeleton(skeleton);
    std::cout << "Number of vertices of the skeleton: " << boost::num_vertices(skeleton) << "\n";
    std::cout << "Number of edges of the skeleton: " << boost::num_edges(skeleton) << "\n";
    // Output all the edges of the skeleton.
    std::ofstream output("skel.cgal");
    BOOST_FOREACH(Skeleton_edge e, edges(skeleton))
    {
      const CgalPoint& s = skeleton[source(e, skeleton)].point;
      const CgalPoint& t = skeleton[target(e, skeleton)].point;
      lato.first.setY(s.y());
      lato.first.setX(s.x());
      lato.first.setZ(s.z());
      lato.second.setX(t.x());
      lato.second.setY(t.y());
      lato.second.setZ(t.z());
      listaLati.push_back(lato);

      //output << "2 "<< s << " " << t << "\n";
    }


    return listaLati;

}
