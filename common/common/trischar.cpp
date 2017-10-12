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

bool trisCharacteristic::isAreaLessThan(Pointd a, Pointd b, Pointd c, double threshold)
{
    double soglia = threshold ;


    double area = getTriangleArea(a,b,c);


    if(area < soglia)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief DrawableSkel::minDistanceBetweenThreePoints calculate minimum distance between points
 * @param a
 * @param b
 * @param c
 * @return 0 if the edge is first-second, 1 if it is first third, -1 if it is second third
 */
int trisCharacteristic::minDistanceBetweenThreePoints(Pointd a, Pointd b, Pointd c)
{
    double distanceAB = a.dist(b);
    double distanceAC = a.dist(c);
    double distanceBC = b.dist(c);

    if(distanceAB < distanceAC)
    {
        if(distanceAB < distanceBC)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        if(distanceAC < distanceBC)
        {
            return 1;

        }
        else
        {
            return -1;
        }
    }


}

/**
 * @brief hasTidThisTwoVertexes
 * @param tA vertex of triangle to be checked
 * @param tB vertex of triangle to be checked
 * @param tC vertex of triangle to be checked
 * @param a  check if is in triangle
 * @param b  check if is in triangle
 * @return true se tid ha a e b come vertici, false altrimenti.
 */
bool trisCharacteristic::hasTidThisTwoVertexes(Pointd a, Pointd b, Pointd tA, Pointd tB, Pointd tC)
{

    if(tA == a)
    {
        if(tB == b || tC == b)
        {
            return true;

        }
        else
        {
            return false;
        }
    }
    else
    {
        if(tA == b)
        {
            if(tB == a || tC == a)
            {
                return true;

            }
            else
            {
                return false;
            }
        }
        if(tB == a)
        {
            if(tC == b)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if(tB == b)
            {
                if(tC == a)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }

}

long double trisCharacteristic::getTriangleArea(Pointd a, Pointd b, Pointd c)
{
    Pointd v1 = b - a;
    Pointd v2 = c - a;

    Pointd cross = v1.cross(v2);
    long double area = cross.getLength()/2;

    return area;
}
