#include "skeletoncreator.h"

SkeletonCreator::SkeletonCreator()
{
    surfaceArea = 0;
}

SkeletonCreator::SkeletonCreator(const char* input)
{
    surfaceArea = 0;
    computeSkeleton(input);
}

SkeletonCreator::SkeletonCreator(const char* input, int numCompressions)
{
    surfaceArea = 0;
    computeSkeleton(input, numCompressions);
}

void SkeletonCreator::computeSkeleton(const char* path)
{
    std::ifstream input(path);
    std::pair<Pointd, Pointd> lato;
    ///punti temp triangoli
    Pointd a, b, c;
    //skelComponents edgesTAndCoords;

    //aree temp;
    //double areaTenere = 0.0, areaPrev = 0.0;
    //Skeletonization mcsTenere, mcsPrev;

    std::list<std::pair<Pointd, Pointd>> listaLati;
    Triangle_mesh tmesh;
    input >> tmesh;
    //Skeleton skeleton;
//    Skel mySkel;
//    Pointd third;
    int i=0;

    std::vector<int> triangles;
    std::vector<double> coord;
    QVector<Pointd> tempPoints;
    QVector<Pointd> tempPointsNoPos;

    Skeletonization mcs(tmesh);
//    Skeletonization mcsPrev = mcs;
//    Skeletonization mcsTenere = mcs;


    for(int i=0; i<2; i++)
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
//    double originalArea = 0;
//    while ((areaTenere-surfaceArea) >= (originalArea*mcs.area_variation_factor()) || i < 2 )
//    {

//        surfaceArea = 0.0;
//        // 1. Contract the mesh by mean curvature flow.
//        mcs.contract_geometry();
//        // 2. Collapse short edges and split bad triangles.
//        mcs.collapse_edges();
//        mcs.split_faces();
//        // 3. Fix degenerate vertices.
//        mcs.detect_degeneracies();
//        // Perform the above three steps in one iteration.
//        mcs.contract();
//        Meso_skeleton mesoSkeleton = mcs.meso_skeleton();
//        for( Meso_skeleton::Facet_const_iterator face = mesoSkeleton.facets_begin(); face != mesoSkeleton.facets_end(); ++face )
//        {
//            a.setX(face->halfedge()->vertex()->point().x());
//            a.setY(face->halfedge()->vertex()->point().y());
//            a.setZ(face->halfedge()->vertex()->point().z());
//            b.setX(face->halfedge()->next()->vertex()->point().x());
//            b.setY(face->halfedge()->next()->vertex()->point().y());
//            b.setZ(face->halfedge()->next()->vertex()->point().z());
//            c.setX(face->halfedge()->next()->next()->vertex()->point().x());
//            c.setY(face->halfedge()->next()->next()->vertex()->point().y());
//            c.setZ(face->halfedge()->next()->next()->vertex()->point().z());
//    //        qDebug() << "Is triangle? " << (face->is_triangle()) << "\n";
//    //        qDebug() << "Is quad? " << (face->is_quad()) << "\n";

//            surfaceArea += getTriangleArea(a,b,c);
//        }

//        if(i == 0)
//        {
//            originalArea = surfaceArea;
//        }
//        i++;

//        areaTenere = areaPrev;
//        areaPrev = surfaceArea;


//            qDebug() << "areaPrev - surfaceArea = " << (areaPrev-surfaceArea)
//                     << " areaPrev*factor = " << (areaPrev*mcs.area_variation_factor());
//            qDebug() << "areaPrev = " <<areaPrev;

//        mcsTenere = mcsPrev;
//        mcsPrev = mcs;

//    }

    i = 0;
    // Iteratively apply step 1 to 3 until convergence.
    //mcs.contract_until_convergence();
    //Meso_skeleton mesoSkeleton = mcsTenere.meso_skeleton();
    Meso_skeleton mesoSkeleton = mcs.meso_skeleton();

    for( Meso_skeleton::Facet_const_iterator face = mesoSkeleton.facets_begin(); face != mesoSkeleton.facets_end(); ++face )
    {
        Meso_skeleton::Halfedge_const_handle begin = face->halfedge();

        a.setX(face->halfedge()->vertex()->point().x());
        a.setY(face->halfedge()->vertex()->point().y());
        a.setZ(face->halfedge()->vertex()->point().z());
        b.setX(face->halfedge()->next()->vertex()->point().x());
        b.setY(face->halfedge()->next()->vertex()->point().y());
        b.setZ(face->halfedge()->next()->vertex()->point().z());
        c.setX(face->halfedge()->next()->next()->vertex()->point().x());
        c.setY(face->halfedge()->next()->next()->vertex()->point().y());
        c.setZ(face->halfedge()->next()->next()->vertex()->point().z());
//        qDebug() << "Is triangle? " << (face->is_triangle()) << "\n";
//        qDebug() << "Is quad? " << (face->is_quad()) << "\n";

        surfaceArea += getTriangleArea(a,b,c);

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

void SkeletonCreator::computeMesoSkeleton(const char* path)
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

void SkeletonCreator::compressMesoSkeleton(std::vector<int>* tris, std::vector<double>* points, double trashold)
{
//    for(int tid=0; tid<n_tris; ++tid) {
//        int tid_ptr  = 3 * tid;
//        int vid0     = tris[tid_ptr + 0];
//        int vid1     = tris[tid_ptr + 1];
//        int vid2     = tris[tid_ptr + 2];
//        int vid0_ptr = 3 * vid0;
//        int vid1_ptr = 3 * vid1;
//        int vid2_ptr = 3 * vid2;
//        int j=0;
//        haveNeighborsSmallArea = true;
//        if(isTrisOnBorder2(Pointd(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]),
//                          Pointd(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]),
//                          Pointd(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2]),
//                          triTrashold)||
//           isTrisOnBorder(Pointd(coords[vid0_ptr],coords[vid0_ptr+1],coords[vid0_ptr+2]),
//                           Pointd(coords[vid1_ptr],coords[vid1_ptr+1],coords[vid1_ptr+2]),
//                           Pointd(coords[vid2_ptr],coords[vid2_ptr+1],coords[vid2_ptr+2]),
//                           15))
//        {

//            //controllo se i vicini hanno area piccola anche loro tramite
//            //i vettori tri2tri "riempiti" da metodi già esistenti come
//            //buildAdjagency. In questo caso, se un triangolo ha almeno 2 vicini
//            //che hanno area piccola, allora lo coloro di rosso, altrimenti di blu.
//            for(int i=0; i < tri2tri[tid].size(); i++)
//            {

//                haveNeighborsSmallArea = true;
//                int tid_ptr0  = 3 * (tri2tri[tid])[i];
//                int vid00     = tris[tid_ptr0 + 0];
//                int vid10     = tris[tid_ptr0 + 1];
//                int vid20     = tris[tid_ptr0 + 2];
//                int vid0_ptr0 = 3 * vid00;
//                int vid1_ptr0 = 3 * vid10;
//                int vid2_ptr0 = 3 * vid20;
//                //controllo l'area del vicino, presuppongo che i vicini siano 3 (constatato che non è sicuro)
//                if(!isTrisOnBorder2(Pointd(coords[vid0_ptr0],coords[vid0_ptr0+1],coords[vid0_ptr0+2]),
//                                  Pointd(coords[vid1_ptr0],coords[vid1_ptr0+1],coords[vid1_ptr0+2]),
//                                  Pointd(coords[vid2_ptr0],coords[vid2_ptr0+1],coords[vid2_ptr0+2]),
//                                    triTrashold))
//                {

//                    j++;
//                }

//                if(j > 1)
//                {
//                    haveNeighborsSmallArea = false;
//                }
//            }
}

double SkeletonCreator::getTriangleArea(Pointd a, Pointd b, Pointd c)
{

    Pointd v1 = b - a;
    Pointd v2 = c - a;

    Pointd cross = v1.cross(v2);
    double area = cross.getLength()/2;

    return area;

}
/*
 * Calcola il meso skeleton in base al numero di compressioni date come parametro
 */
void SkeletonCreator::computeSkeleton(const char* path, int numCompressions)
{
    std::ifstream input(path);
    std::pair<Pointd, Pointd> lato;

    ///punti temp triangoli
    Pointd a, b, c;

    std::list<std::pair<Pointd, Pointd>> listaLati;
    Triangle_mesh tmesh;
    input >> tmesh;
    int i=0;

    std::vector<int> triangles;
    std::vector<double> coord;
    QVector<Pointd> tempPoints;
    QVector<Pointd> tempPointsNoPos;

    Skeletonization mcs(tmesh);

    for(int i=0; i<numCompressions; i++)
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

    Meso_skeleton mesoSkeleton = mcs.meso_skeleton();

    for( Meso_skeleton::Facet_const_iterator face = mesoSkeleton.facets_begin(); face != mesoSkeleton.facets_end(); ++face )
    {
        Meso_skeleton::Halfedge_const_handle begin = face->halfedge();

        a.setX(face->halfedge()->vertex()->point().x());
        a.setY(face->halfedge()->vertex()->point().y());
        a.setZ(face->halfedge()->vertex()->point().z());
        b.setX(face->halfedge()->next()->vertex()->point().x());
        b.setY(face->halfedge()->next()->vertex()->point().y());
        b.setZ(face->halfedge()->next()->vertex()->point().z());
        c.setX(face->halfedge()->next()->next()->vertex()->point().x());
        c.setY(face->halfedge()->next()->next()->vertex()->point().y());
        c.setZ(face->halfedge()->next()->next()->vertex()->point().z());
//        qDebug() << "Is triangle? " << (face->is_triangle()) << "\n";
//        qDebug() << "Is quad? " << (face->is_quad()) << "\n";

        surfaceArea += getTriangleArea(a,b,c);

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

    //saveObj("/home/pietro/Desktop/prova.obj", coord, triangles);
    listaEdge = listaLati;
    listaPunti = coord;
    listaTriangoli = triangles;



}
