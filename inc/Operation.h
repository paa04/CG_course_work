//
// Created by paa04 on 11.12.24.
//

#ifndef OPERATION_H
#define OPERATION_H
#include <Transform.h>

#include "Terrain.h"
#include "Terrain_manager.h"


using namespace std;

class Operation
{
protected:
    bool isCamera;

    QVector3D light_pos_after_op;

public:
    virtual void do_op(Terrain &terrain, Light &light) = 0;
    virtual void undo(Terrain &terrain, Light &light) = 0;
    virtual void undo_light(Light& light) = 0;

    Operation(bool is_cam, QVector3D lPos): isCamera(is_cam), light_pos_after_op(lPos){}

    virtual ~Operation() = default;
};

class scale_operation : public Operation
{
private:
    scale_data data_;
    QVector3D center_;

public:
    scale_operation(scale_data data, QVector3D center, bool is_cam, QVector3D pos): data_(data), center_(center), Operation(is_cam, pos)
    {
    };

    void do_op(Terrain &terrain, Light &light) override;

    void undo(Terrain &terrain, Light &light) override;

    void undo_light(Light &light) override;
};

class rotate_operation : public Operation
{
private:
    rotate_axis axis;
    QVector3D center_;

public:
    rotate_operation(rotate_axis data, QVector3D center, bool is_cam, QVector3D pos): axis(data), center_(center),  Operation(is_cam, pos)
    {
    };

    void do_op(Terrain &terrain, Light &light) override;

    void undo(Terrain &terrain, Light &light) override;

    void undo_light(Light &light) override;
};

class move_operation : public Operation
{
private:
    move_data data;

public:
    move_operation(move_data data, bool is_cam, QVector3D pos): data(data), Operation(is_cam, pos)
    {
    };

    void do_op(Terrain &terrain, Light &light) override;

    void undo(Terrain &terrain, Light &light) override;
    void undo_light(Light &light) override;
};

#endif //OPERATION_H
