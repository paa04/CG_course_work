//
// Created by paa04 on 18.11.24.
//

#include "Light_timer.h"

#include "Terrain_manager.h"

Light_timer::Light_timer(Light &light, QVector3D center):
                                           light(light),
                                           light_center(center)
{
}


void Light_timer::updateLightPosition()
{
    auto l = light.getPosition();
    Transform::rotate(l, light_center, {100, 0, 0});
    light.setPosition(l);
}

