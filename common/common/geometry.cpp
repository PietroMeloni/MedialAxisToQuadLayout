#include "common/geometry.h"

double Geometria::getTriangleArea(Pointd a, Pointd b, Pointd c)
{

    Pointd v1 = b - a;
    Pointd v2 = c - a;

    Pointd cross = v1.cross(v2);
    double area = cross.getLength()/2;

    return area;

}
