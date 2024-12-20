//
// Created by paa04 on 31.10.24.
//

#ifndef PLAIN_H
#define PLAIN_H

#include <QVector3D>

class Plane
{
private:
    QVector3D p1, p2, p3;
    double A, B, C, D;

    QVector2D _pMax, _pMin;
    QVector3D _pMid;

    void calcPlaneCoeff();

    void defCircumRect();

    void calcMidP();

public:
    Plane(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3): p1(p1), p2(p2), p3(p3)
    {
        calcPlaneCoeff();
        defCircumRect();
        calcMidP();
    }

    QVector3D get_midP() const {return _pMid;}

    QVector3D getP1() const { return p1; };
    QVector3D getP2() const { return p2; };
    QVector3D getP3() const { return p3; };

    QVector2D getPMin() const { return _pMin; }
    QVector2D getPMax() const { return _pMax; }

    double getA() const;

    double getB() const;

    double getC() const;

    double getD() const;

    double calcZ(double x, double y) const;

};


#endif //PLAIN_H
