#ifndef TRISCHAR_H
#define TRISCHAR_H

#endif // TRISCHAR_H
#include <math.h>
#include "point.h"
class trisCharacteristic
{
public:
    trisCharacteristic();
    static double getTriangleArea(Pointd a, Pointd b, Pointd c);
    static double getTriangleMinAngle(Pointd a, Pointd b, Pointd c);
};


