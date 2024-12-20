//
// Created by paa04 on 10.12.24.
//

#include "Camera.h"

Camera::Camera():_vec({0,0, -1})
{
}

QVector3D &Camera::get_vec()
{
    return _vec;
}

const QVector3D &Camera::get_vec() const
{
    return _vec;
}


void Camera::set_vec(QVector3D &vec)
{
    _vec = vec;
}
