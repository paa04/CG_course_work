//
// Created by paa04 on 31.10.24.
//

#ifndef RENDER_H
#define RENDER_H
#include "Terrain.h"
#include "Plane.h"

#include <QImage>

class QGraphicsScene;
template<typename T>
using Matrix = vector<vector<T>>;

struct Pixel_buff
{
    QVector3D vec;
    double I;
};

class Render {
private:
    vector<Plane> plain_model;
    vector<double> plane_h;

    int screenWidth, screenHeight;

    Matrix<double> zbuffer;
    QImage framebuffer;

    int _getCorrectChannel(int channel, double I);

    void get_correct_color(QColor &color, double I);
    QColor _getColorByHeight(double I, double height, double maxHeight, double waterlevel);

    void _calcParamsLine(vector<Pixel_buff> &line, const double &ZPStart, const double &ZPEnd);

    void get_planes_model(const Terrain &terrain);
    vector<Plane> get_planes();

    void render_plane(const Plane& plane, double maxHeight, double minHeight, double plane_real_h, double intensity, double water_level);
    vector<Pixel_buff> _getLineByBresenham(const QVector3D &p1, const QVector3D &p2);

public:
    Render():Render(1000, 1000){}
    Render(int screenW, int screenH);

    void render_terrain(const Terrain &terrain, QGraphicsScene *scene);

    void clean();

};



#endif //RENDER_H
