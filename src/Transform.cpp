//
// Created by paa04 on 18.11.24.
//

#include "Transform.h"

double Transform::toRadians(const double angle)
{
    return angle * (M_PI / 180);
}

void Transform::rotateByX(QVector3D &point, double angle, const QVector3D &centerRotate)
{
    double Yc = centerRotate.y();
    double Zc = centerRotate.z();

    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_y = point.y();

    point.setY(Yc + (point.y() - Yc) * cos_angle + (point.z() - Zc) * sin_angle);
    point.setZ(Zc + (Yc - tmp_y) * sin_angle + (point.z() - Zc) * cos_angle);
}

void Transform::rotateByY(QVector3D &point, double angle, const QVector3D &centerRotate)
{
    double Xc = centerRotate.x();
    double Zc = centerRotate.z();

    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_x = point.x();

    point.setX(Xc + (point.x() - Xc) * cos_angle - (point.z() - Zc) * sin_angle);
    point.setZ(Zc + (tmp_x - Xc) * sin_angle + (point.z() - Zc) * cos_angle);
}

void Transform::rotateByZ(QVector3D &point, double angle, const QVector3D &centerRotate)
{
    double Xc = centerRotate.x();
    double Yc = centerRotate.y();

    double cos_angle = cos(toRadians(angle));
    double sin_angle = sin(toRadians(angle));

    double tmp_x = point.x();

    point.setX(Xc + (point.x() - Xc) * cos_angle + (point.y() - Yc) * sin_angle);
    point.setY(Yc + (Xc - tmp_x) * sin_angle + (point.y() - Yc) * cos_angle);
}

void Transform::rotate(QVector3D &point, const QVector3D &center, const rotate_axis &axis)
{
    rotateByX(point, axis.axis_x, center);
    rotateByY(point, axis.axis_y, center);
    rotateByZ(point, axis.axis_z, center);
}

void Transform::move(QVector3D &point, const move_data &move)
{
    point.setX(point.x() + move.dx);
    point.setY(point.y() + move.dy);
    point.setZ(point.z() + move.dz);
}


void Transform::scale(QVector3D &point, const QVector3D &center, const scale_data &scale)
{
    if (scale.kx * scale.ky * scale.kz > 0.001)
    {
        point.setX(scale.kx * point.x() + (1 - scale.kx) * center.x());
        point.setY(scale.ky * point.y() + (1 - scale.ky) * center.y());
        point.setZ(scale.kz * point.z() + (1 - scale.kz) * center.z());
    }
}
