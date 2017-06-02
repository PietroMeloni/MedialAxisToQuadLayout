#include "cgalskeletoninterface.h"

CgalSkeletonInterface::CgalSkeletonInterface()
{

}

CgalSkeletonInterface::CgalSkeletonInterface(const char* input)
{
    computeSkeleton(input);
}

void CgalSkeletonInterface::computeSkeleton(const char* path)
{
    std::ifstream input(path);
    std::pair<Pointd, Pointd> lato;

    //skelComponents edgesTAndCoords;

    std::list<std::pair<Pointd, Pointd>> listaLati;
    Triangle_mesh tmesh;
    input >> tmesh;
    //Skeleton skeleton;
    Skel mySkel;
    Pointd third;
    int i=0;

    std::vector<int> triangles;
    std::vector<double> coord;
    QVector<Pointd> tempPoints;
    QVector<Pointd> tempPointsNoPos;

    Skeletonization mcs(tmesh);


    for(int i=0; i<1; i++)
    {

    // 1. Contract the mesh by mean curvature flow.
    mcs.contract_geometry();
    // 2. Collapse short edges and split bad triangles.
    mcs.collapse_edges();
    mcs.split_faces();
    // 3. Fix degenerate vertices.
    mcs.detect_degeneracies();
    // Perform the above three steps in one iteration.
    mcs.contract();
    }
    // Iteratively apply step 1 to 3 until convergence.
    //mcs.contract_until_convergence();
    Meso_skeleton mesoSkeleton = mcs.meso_skeleton();

    for( Meso_skeleton::Facet_const_iterator face = mesoSkeleton.facets_begin(); face != mesoSkeleton.facets_end(); ++face )
    {
        Meso_skeleton::Halfedge_const_handle begin = face->halfedge();
//        qDebug() << "Is triangle? " << (face->is_triangle()) << "\n";
//        qDebug() << "Is quad? " << (face->is_quad()) << "\n";



        Meso_skeleton::Halfedge_const_handle hch = begin;
        do
        {
            i++;
            lato.first.setY(hch->vertex()->point().y());
            lato.first.setX(hch->vertex()->point().x());
            lato.first.setZ(hch->vertex()->point().z());

            if(!tempPoints.contains(lato.first))
            {
                tempPoints.push_back(lato.first);
                coord.push_back(lato.first.x());
                coord.push_back(lato.first.y());
                coord.push_back(lato.first.z());
            }

            tempPointsNoPos.push_back(lato.first);

            lato.second.setX(hch->next()->vertex()->point().x());
            lato.second.setY(hch->next()->vertex()->point().y());
            lato.second.setZ(hch->next()->vertex()->point().z());
            if(!tempPoints.contains(lato.second))
            {
                tempPoints.push_back(lato.second);
                coord.push_back(lato.second.x());
                coord.push_back(lato.second.y());
                coord.push_back(lato.second.z());
            }


            hch = hch->next();
            //triangles.push_back(tempPoints.indexOf(lato.second));
            listaLati.push_back(lato);
        }
        while(hch != begin);
        triangles.push_back(tempPoints.indexOf(tempPointsNoPos[tempPointsNoPos.size()-3]));
        triangles.push_back(tempPoints.indexOf(tempPointsNoPos[tempPointsNoPos.size()-2]));
        triangles.push_back(tempPoints.indexOf(tempPointsNoPos[tempPointsNoPos.size()-1]));


    }

    saveObj("/home/pietro/Desktop/prova.obj", coord, triangles);

    listaEdge = listaLati;
    listaPunti = coord;
    listaTriangoli = triangles;



}

void CgalSkeletonInterface::computeMesoSkeleton(const char* path)
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


    //return listaLati;

}


