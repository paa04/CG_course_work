//
// Created by paa04 on 18.11.24.
//

#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <QVector3D>

typedef struct
{
    double axis_x;
    double axis_y;
    double axis_z;
} rotate_axis;

typedef struct
{
    double dx;
    double dy;
    double dz;
} move_data;

typedef struct
{
    double kx;
    double ky;
    double kz;
} scale_data;

class Transform {
private:
    static double toRadians(double angle);
    static void rotateByX(QVector3D &point, const double angle,const QVector3D &centerRotate);
    static void rotateByY(QVector3D &point, const double angle,const QVector3D &centerRotate);
    static void rotateByZ(QVector3D &point, const double angle,const QVector3D &centerRotate);

    public:
    static void rotate(QVector3D& point, const QVector3D& center, const rotate_axis &axis);
    static void move(QVector3D& point, const move_data& move);
    static void scale(QVector3D& point, const QVector3D& center, const scale_data& scale);
};



#endif //TRANSFORM_H
