//
// Created by paa04 on 31.10.24.
//

#include "Plane.h"

void Plane::calcPlaneCoeff()
{
    double m = p2.x() - p1.x();
    double n = p2.y() - p1.y();
    double p = p2.z() - p1.z();

    double k = p3.x() - p1.x();
    double s = p3.y() - p1.y();
    double e = p3.z() - p1.z();

    A = n * e - s * p;
    B = k * p - m * e;
    C = m * s - k * n;

    if (C < 0)
    {
        A = -A;
        B = -B;
        C = -C;
    }

    D = -(A * p1.x() + B * p1.y() + C * p1.z());
}

void Plane::defCircumRect()
{
    _pMin.setX(std::min({p1.x(), p2.x(), p3.x()}));
    _pMin.setY(std::min({p1.y(), p2.y(), p3.y()}));

    _pMax.setX(std::max({p1.x(), p2.x(), p3.x()}));
    _pMax.setY(std::max({p1.y(), p2.y(), p3.y()}));
}

double Plane::getA() const
{
    return A;
}

double Plane::getB() const
{
    return B;
}

double Plane::getC() const
{
    return C;
}

double Plane::getD() const
{
    return D;
}

double Plane::calcZ(double x, double y) const
{
    return -(A * x + B * y + D) / C;
}


void Plane::calcMidP()
{
    float x = p1.x() + p2.x() + p3.x();
    float y = p1.y() + p2.y() + p3.y();
    float z = p1.z() + p2.z() + p3.z();

    _pMid = QVector3D{x / 3, y / 3, z / 3};
}
