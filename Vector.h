#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
    Vector(double x, double y);
    double x;
    double y;
    void add(Vector vector);
    Vector subtract(Vector vector);
    void mult(double scalar);
    void div(double scalar);
    double mag();
    void setMag(double mag);
    void normalize();
    void limit(double max);
};

#endif