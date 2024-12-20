//
// Created by paa04 on 11.12.24.
//

#include "Operation.h"

void move_operation::undo(Terrain &terrain, Light &light)
{
    move_data undo_data = data;
    undo_data.dx = -data.dx;
    undo_data.dy = -data.dy;
    undo_data.dz = -data.dz;

    Terrain_manager::move_terrain(terrain, undo_data);

    if(isCamera)
    {
        auto l_pos_before_op = light_pos_after_op;
        Transform::move(l_pos_before_op, undo_data);
        light.setPosition(l_pos_before_op);
    }
}

void move_operation::undo_light(Light &light)
{
    move_data undo_data = data;
    undo_data.dx = -data.dx;
    undo_data.dy = -data.dy;
    undo_data.dz = -data.dz;

    auto pos = light.getPosition();
    Transform::move(pos, undo_data);
    light.setPosition(pos);
}

void rotate_operation::undo(Terrain &terrain, Light &light)
{
    rotate_axis undo_data = axis;
    undo_data.axis_x = -axis.axis_x;
    undo_data.axis_y = -axis.axis_y;
    undo_data.axis_z = -axis.axis_z;

    Terrain_manager::rotate(terrain, center_, undo_data);

    if(isCamera)
    {
        auto l_pos_before_op = light_pos_after_op;
        Transform::rotate(l_pos_before_op, center_, undo_data);
        light.setPosition(l_pos_before_op);
    }
}


void rotate_operation::undo_light(Light &light)
{
    rotate_axis undo_data = axis;
    undo_data.axis_x = -axis.axis_x;
    undo_data.axis_y = -axis.axis_y;
    undo_data.axis_z = -axis.axis_z;


    auto pos = light.getPosition();
    Transform::rotate(pos, center_, undo_data);
    light.setPosition(pos);
}

void scale_operation::undo(Terrain &terrain, Light &light)
{
    scale_data undo_data = data_;
    undo_data.kx = 1 / data_.kx;
    undo_data.ky = 1 / data_.ky;
    undo_data.kz = 1 / data_.kz;

    Terrain_manager::scale_terrain(terrain, center_, undo_data);

    if(isCamera)
    {
        auto l_pos_before_op = light_pos_after_op;
        Transform::scale(l_pos_before_op, center_, undo_data);
        light.setPosition(l_pos_before_op);
    }
}

void scale_operation::undo_light(Light &light)
{
    scale_data undo_data = data_;
    undo_data.kx = 1 / data_.kx;
    undo_data.ky = 1 / data_.ky;
    undo_data.kz = 1 / data_.kz;

    auto pos = light.getPosition();
    Transform::scale(pos, center_, undo_data);
    light.setPosition(pos);
}

void move_operation::do_op(Terrain &terrain, Light &light)
{
    Terrain_manager::move_terrain(terrain, data);

    if(isCamera)
    {
        light.setPosition(light_pos_after_op);
    }
}

void scale_operation::do_op(Terrain &terrain, Light &light)
{
    Terrain_manager::scale_terrain(terrain, center_, data_);

    if(isCamera)
    {
        light.setPosition(light_pos_after_op);
    }
}

void rotate_operation::do_op(Terrain &terrain, Light &light)
{
    Terrain_manager::rotate(terrain, center_, axis);

    if(isCamera)
    {
        light.setPosition(light_pos_after_op);
    }
}
