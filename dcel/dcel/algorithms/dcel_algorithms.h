#ifndef DCELALGORITHMS_H
#define DCELALGORITHMS_H

#include <dcel/dcel.h>

namespace DcelAlgorithms {
    void getVectorFaces(std::vector<const Dcel::Face*> &vector, const Dcel& d);
    void getVectorFaces(std::vector<Dcel::Face*> &vector, Dcel& d);

    static std::vector<const Dcel::Vertex*> dummymv;
    static std::vector<const Dcel::Face*> dummymf;
    void getVectorMesh(std::vector< Pointd > &coords, std::vector< std::vector<int>> &faces, const Dcel &d, std::vector<const Dcel::Vertex*> &mappingVertices = dummymv, std::vector<const Dcel::Face*> &mappingFaces = dummymf);

    void smartColoring(Dcel &d);
}

#endif // DCELALGORITHMS_H
