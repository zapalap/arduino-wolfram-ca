#include "Vector.h"
#include "math.h"

Vector::Vector(double x, double y)
{
    this->x = x;
    this->y = y;
}

void Vector::add(Vector vector)
{
    x += vector.x;
    y += vector.y;
}

Vector Vector::subtract(Vector vector)
{
    return *(new Vector(x - vector.x, y - vector.y));
}

void Vector::mult(double scalar)
{
    x = x * scalar;
    y = y * scalar;
}

void Vector::div(double scalar)
{
    x = x / scalar;
    y = y / scalar;
}

double Vector::mag()
{
    return sqrt(this->x * this->x + this->y * this->y);
}

void Vector::setMag(double mag)
{
    this->normalize();
    this->mult(mag);
}

void Vector::normalize()
{
    double magnitude = this->mag();
    if (magnitude != 0)
    {
        this->div(magnitude);
    }
}

void Vector::limit(double max)
{
    double magnitude = this->mag();
    if (magnitude > max)
    {
        this->setMag(max);
    }
}
