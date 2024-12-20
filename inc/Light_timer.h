//
// Created by paa04 on 18.11.24.
//

#ifndef LIGHT_TIMER_H
#define LIGHT_TIMER_H
#include <qtmetamacros.h>
#include <QTimer>
#include <Transform.h>

#include "light.h"
#include "Render.h"


class QGraphicsScene;

class Light_timer
{
private:
    Light &light;
    QVector3D light_center;

public:
    Light_timer(Light &light, QVector3D center);

    // void init(Light &light, Terrain &terrain, Render &render, QGraphicsScene *scene, QVector3D center, QObject *parent = nullptr);

    void updateLightPosition();
};


#endif //LIGHT_TIMER_H
