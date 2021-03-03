#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
    Vector();
    Vector(double x1, double y1);
    double x;
    double y;
    void add(const Vector &vector);
    Vector subtract(const Vector &vector);
    void mult(double scalar);
    void div(double scalar);
    double mag();
    void setMag(double mag);
    void normalize();
    void limit(double max);
};

#endif