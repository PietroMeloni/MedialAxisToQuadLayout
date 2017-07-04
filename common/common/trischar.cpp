#include "common/trischar.h"

double trisCharacteristic::getTriangleMinAngle(Pointd a, Pointd b, Pointd c)
{
    Pointd v1 = a - b;
    Pointd v2 = b - c;
    Pointd v3 = a - c;
    v1.normalize();
    v2.normalize();
    v3.normalize();

    double angle1 = acos(v1.dot(v2));
    double angle2 = acos(v1.dot(v3));
    double angle3 = acos(v2.dot(v3));
    double min;

    if(angle1 <= angle2)
    {
        if(angle1 <= angle3)
        {
            min = angle1;
        }
        else
        {
            min = angle3;
        }
    }
    else
    {
        if (angle2 <= angle3)
        {
            min = angle2;
        }
        else
        {
            min = angle3;
        }
    }

    return min;
}

double trisCharacteristic::getTriangleArea(Pointd a, Pointd b, Pointd c)
{
    Pointd v1 = b - a;
    Pointd v2 = c - a;

    Pointd cross = v1.cross(v2);
    double area = cross.getLength()/2;

    return area;
}
