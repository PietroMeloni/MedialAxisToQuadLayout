#ifndef TRISCHAR_H
#define TRISCHAR_H


#include <math.h>
#include "point.h"
class trisCharacteristic
{
public:
    trisCharacteristic();
    static long double getTriangleArea(Pointd a, Pointd b, Pointd c);
    static double getTriangleMinAngle(Pointd a, Pointd b, Pointd c);
    static bool isAreaLessThan(Pointd a, Pointd b, Pointd c, double threshold);
    static int minDistanceBetweenThreePoints(Pointd a, Pointd b, Pointd c);
    static bool hasTidThisTwoVertexes(Pointd a, Pointd b, Pointd tA, Pointd tB, Pointd tC);
};


#endif // TRISCHAR_H
