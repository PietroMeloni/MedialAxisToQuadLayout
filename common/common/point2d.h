#ifndef POINT2D_H
#define POINT2D_H

#include <sstream>
#include <assert.h>
#include <QDebug>
#include <string>
#include <iostream>
#include <math.h>

#include "serialize.h"

template <class T> class Point2D : SerializableObject {
    public:
        Point2D(T x = 0.0, T y = 0.0);
        #ifdef COMMON_WITH_EIGEN
        Point2D(const Eigen::VectorXd &v);
        #endif

        T x() const;
        T y() const;
        double dist(const Point2D<T> &otherPoint) const;
        double dot(const Point2D<T> &otherVector) const;
        double perpendicularDot(const Point2D<T> &otherVector) const;
        double getLength() const;
        double getLengthSquared() const;
        Point2D<T> min(const Point2D<T> &otherPoint) const;
        Point2D<T> max(const Point2D<T> &otherPoint) const;

        const T& operator[](unsigned int i)                 const;
        const T& operator()(unsigned int i)                 const;
        bool operator == (const Point2D<T> &otherPoint)   const;
        bool operator != (const Point2D<T> &otherPoint)   const;
        bool operator < (const Point2D<T>& otherPoint)    const;
        bool operator > (const Point2D<T>& otherPoint)    const;
        bool operator <= (const Point2D<T>& otherPoint)   const;
        bool operator >= (const Point2D<T>& otherPoint)   const;
        Point2D<T> operator - ()                          const;
        Point2D<T> operator + (const T& scalar) const;
        Point2D<T> operator + (const Point2D<T>& otherPoint) const;
        Point2D<T> operator - (const Point2D<T>& otherPoint) const;
        Point2D<T> operator * (const T& scalar)      const;
        Point2D<T> operator * (const Point2D<T>& otherPoint) const;
        Point2D<T> operator / (const T& scalar )     const;
        Point2D<T> operator / (const Point2D<T>& otherPoint) const;

        void setXCoord(const T& x);
        void setYCoord(const T& y);
        void set(const T& x, const T& y);
        double normalize();
        void rotate(T matrix[2][2], Point2D<T> centroid = Point2D<T>());
        Point2D<T> operator += (const Point2D<T>& otherPoint);
        Point2D<T> operator -= (const Point2D<T>& otherPoint);
        Point2D<T> operator *= (const T& scalar);
        Point2D<T> operator *= (const Point2D<T>& otherPoint);
        Point2D<T> operator /= (const T& scalar );
        Point2D<T> operator /= (const Point2D<T>& otherPoint);

    private:
        T xCoord, yCoord;
        void rot(T matrix[2][2]);

        // SerializableObject interface
    public:
        void serialize(std::ofstream& binaryFile) const;
        bool deserialize(std::ifstream& binaryFile);
};

template <class T>
inline void Point2D<T>::rot(T matrix[2][2]){
    Point2D<T> p;
    p.setXCoord(matrix[0][0]*xCoord + matrix[0][1]*yCoord);
    p.setYCoord(matrix[1][0]*xCoord + matrix[1][1]*yCoord);
    xCoord = p.x();
    yCoord = p.y();
}

template <class T>
void Point2D<T>::serialize(std::ofstream& binaryFile) const {
    Serializer::serialize(xCoord, binaryFile);
    Serializer::serialize(yCoord, binaryFile);
}

template <class T>
bool Point2D<T>::deserialize(std::ifstream& binaryFile) {
    Point2D<T> tmp;
    if (Serializer::deserialize(tmp.xCoord, binaryFile) &&
        Serializer::deserialize(tmp.yCoord, binaryFile)){
        *this = std::move(tmp);
        return true;
    }
    else
        return false;
}

typedef Point2D<double> Point2Dd;
typedef Point2D<float> Point2Df;
typedef Point2D<int> Point2Di;
typedef Point2D<double> Vec2;

/****************
* Other Methods *
*****************/

template <class T>
Point2D<T> operator * (const T& scalar, const Point2D<T>& point);

template <class T>
Point2D<T> normalOfSegment(const Point2D<T>& p1, const Point2D<T>& p2);

template <class T>
inline Point2D<T>::Point2D(T x, T y): xCoord(x), yCoord(y) {
}

template <class T>
Point2D<T>::Point2D(const Eigen::VectorXd& v) : xCoord(v(0)), yCoord(v(1)){
}

template <class T>
inline T Point2D<T>::x() const {
    return xCoord;
}

template <class T>
inline T Point2D<T>::y() const {
    return yCoord;
}

template <class T>
inline double Point2D<T>::dist(const Point2D<T> &otherPoint) const{
    return sqrt ( pow((xCoord - otherPoint.xCoord), 2) +
                  pow((yCoord - otherPoint.yCoord), 2) );
}

template <class T>
inline double Point2D<T>::dot(const Point2D<T> &otherVector) const{
    return xCoord * otherVector.xCoord +
           yCoord * otherVector.yCoord;
}

template <class T>
inline double Point2D<T>::perpendicularDot(const Point2D<T>& otherVector) const {
    return xCoord * otherVector.yCoord -
           yCoord * otherVector.xCoord;
}

template <class T>
inline double Point2D<T>::getLength() const{
    return sqrt( xCoord*xCoord + yCoord*yCoord );
}

template <class T>
inline double Point2D<T>::getLengthSquared() const{
    return xCoord*xCoord + yCoord*yCoord;
}

template <class T>
inline Point2D<T> Point2D<T>::min(const Point2D<T> &otherPoint) const{
    return Point2D<T>(std::min(x(), otherPoint.xCoord),
                   std::min(y(), otherPoint.yCoord));
}

template <class T>
inline Point2D<T> Point2D<T>::max(const Point2D<T> &otherPoint) const{
    return Point2D<T>(std::max(x(), otherPoint.xCoord),
                      std::max(y(), otherPoint.yCoord));
}

template <class T>
const T& Point2D<T>::operator[](unsigned int i) const {
    assert(i < 2);
    switch (i){
        case 0: return xCoord;
        case 1: return yCoord;
    }
    return xCoord;
}

template <class T>
const T&Point2D<T>::operator()(unsigned int i) const {
    assert(i < 2);
    switch (i){
        case 0: return xCoord;
        case 1: return yCoord;
    }
    return xCoord;
}

template <class T>
inline bool Point2D<T>::operator == (const Point2D<T>& otherPoint) const {
    if ( otherPoint.xCoord != xCoord )	return false;
    if ( otherPoint.yCoord != yCoord )	return false;
    return true;
}

template <class T>
inline bool Point2D<T>::operator != (const Point2D<T>& otherPoint) const {
    if ( otherPoint.xCoord != xCoord )	return true;
    if ( otherPoint.yCoord != yCoord )	return true;
    return false;
}

template <class T>
inline bool Point2D<T>::operator < (const Point2D<T>& otherPoint) const {
    if (this->xCoord < otherPoint.xCoord) return true;
    if (this->xCoord > otherPoint.xCoord) return false;
    if (this->yCoord < otherPoint.yCoord) return true;
    return false;
}

template <class T>
inline bool Point2D<T>::operator >(const Point2D<T>& otherPoint) const {
    if (this->xCoord < otherPoint.xCoord) return false;
    if (this->xCoord > otherPoint.xCoord) return true;
    if (this->yCoord > otherPoint.yCoord) return true;
    return false;
}

template <class T>
inline bool Point2D<T>::operator <=(const Point2D<T>& otherPoint) const {
    return *this == otherPoint || *this < otherPoint;
}

template <class T>
inline bool Point2D<T>::operator >=(const Point2D<T>& otherPoint) const {
    return *this == otherPoint || *this > otherPoint;
}

template <class T>
inline Point2D<T> Point2D<T>::operator - () const {
    return Point2D(-xCoord, -yCoord);
}

template <class T>
Point2D<T> Point2D<T>::operator +(const T& scalar) const {
    return Point2D<T>(xCoord + scalar,
                      yCoord + scalar);
}

template <class T>
inline Point2D<T> Point2D<T>::operator + (const Point2D<T>& otherPoint) const {
    return Point2D(xCoord + otherPoint.xCoord,
                    yCoord + otherPoint.yCoord);
}

template <class T>
inline Point2D<T> Point2D<T>::operator - (const Point2D<T>& otherPoint) const {
    return Point2D(xCoord - otherPoint.xCoord,
                    yCoord - otherPoint.yCoord);
}

template <class T>
inline Point2D<T> Point2D<T>::operator * (const T& scalar) const {
    return Point2D(xCoord * scalar, yCoord * scalar);
}

template <class T>
inline Point2D<T> Point2D<T>::operator * (const Point2D<T>& otherPoint) const {
    return Point2D(xCoord * otherPoint.xCoord, yCoord * otherPoint.yCoord);
}

template <class T>
inline Point2D<T> Point2D<T>::operator / (const T& scalar) const {
    return Point2D(xCoord / scalar, yCoord / scalar);
}

template <class T>
inline Point2D<T> Point2D<T>::operator / (const Point2D<T>& otherPoint) const {
    return Point2D(xCoord / otherPoint.xCoord, yCoord / otherPoint.yCoord);
}

template <class T>
inline void Point2D<T>::setXCoord(const T& x) {
    xCoord = x;
}

template <class T>
inline void Point2D<T>::setYCoord(const T& y) {
    yCoord = y;
}

template <class T>
inline void Point2D<T>::set(const T& x, const T& y) {
    xCoord = x;
    yCoord = y;
}

template <class T>
inline double Point2D<T>::normalize() {
    double len = getLength();
    xCoord /= len;
    yCoord /= len;
    return len;
}

template <class T>
inline void Point2D<T>::rotate(T matrix[2][2], Point2D centroid){
    *this -= centroid;
    rot(matrix);
    *this += centroid;
}

template <class T>
inline Point2D<T> Point2D<T>::operator += (const Point2D<T>& otherPoint) {
    xCoord += otherPoint.xCoord;
    yCoord += otherPoint.yCoord;
    return *this;
}

template <class T>
inline Point2D<T> Point2D<T>::operator -= (const Point2D<T>& otherPoint) {
    xCoord -= otherPoint.xCoord;
    yCoord -= otherPoint.yCoord;
    return *this;
}

template <class T>
inline Point2D<T> Point2D<T>::operator *= (const T& scalar) {
    xCoord *= scalar;
    yCoord *= scalar;
    return *this;
}

template <class T>
inline Point2D<T> Point2D<T>::operator *= (const Point2D<T>& otherPoint) {
    xCoord *= otherPoint.xCoord;
    yCoord *= otherPoint.yCoord;
    return *this;
}

template <class T>
inline Point2D<T> Point2D<T>::operator /= (const T& scalar) {
    xCoord /= scalar;
    yCoord /= scalar;
    return *this;
}

template <class T>
inline Point2D<T> Point2D<T>::operator /= (const Point2D<T>& otherPoint) {
    xCoord /= otherPoint.xCoord;
    yCoord /= otherPoint.yCoord;
    return *this;
}

template <class T>
inline Point2D<T> operator *(const T& scalar, const Point2D<T>& point) {
    return Point2D<T>(point.x() * scalar,
                   point.y() * scalar);
}

template <class T>
inline Point2D<T> normalOfSegment(const Point2D<T>& p1, const Point2D<T>& p2) {
    double matrix[2][2] = {{0,-1},
                           {1,0}};

    Point2D<T> p = p1 - p2;
    p.normalize();
    p.rotate(matrix);
    return p;
}

template <class T>
inline double area(const Point2D<T>& p0, const Point2D<T>& p1, const Point2D<T>& p2){
    //return (p1.x() - p0.x())*(p2.y()-p0.y()) - ((p2.x()-p0.x())*(p1.y()-p0.y()))/2;
    double A = -(p1.y() - p0.y());
    double B = p1.x() - p0.x();
    double C = -(A * p0.x() + B * p0.y());
    return A * p2.x() + B * p2.y() + C;
}

#endif // POINT2D_H
