//
// Created by paa04 on 09.12.24.
//

#pragma once

#include <QVector3D>

void inline correct_normal(const QVector3D &corrector, QVector3D &normal)
{
    auto screen_corector_ = corrector.normalized();
     // screen_corector_.setY(-screen_corector_.y());
    if (QVector3D::dotProduct(screen_corector_, normal) < 0)
        normal = -normal;
}
