//
// Created by paa04 on 31.10.24.
//

#include "Render.h"

#include <fstream>
#include <qgraphicsscene.h>

#include "Color.h"


Render::Render(int screenW, int screenH): screenWidth(screenW), screenHeight(screenH),
                                          zbuffer(screenW, vector<double>(screenH, INT_MIN)),
                                          framebuffer(screenWidth, screenHeight, QImage::Format_RGB32)
{
}


void Render::get_planes_model(const Terrain &terrain)
{
    // std::ofstream out;
    // out.open("log_ter.txt");
    // out << terrain;
    // out.close();

    plain_model.clear();

    auto &s_mtr = terrain.get_screen_matrix();
    auto &h_mtr = terrain.get_height_matrix();

    for (int i = 0; i < terrain.size() - 1; ++i)
        for (int j = 0; j < terrain.size() - 1; ++j)
        {
            // QVector3D p1(i, j, terrain[i][j]);
            // QVector3D p2(i + 1, j, terrain[i + 1][j]);
            // QVector3D p3(i, j + 1, terrain[i][j + 1]);
            // QVector3D p4(i + 1, j + 1, terrain[i + 1][j + 1]);

            Plane plane1(s_mtr[i][j], s_mtr[i + 1][j], s_mtr[i + 1][j + 1]);
            Plane plane2(s_mtr[i][j], s_mtr[i][j + 1], s_mtr[i + 1][j + 1]);
            plain_model.push_back(plane1);
            plain_model.push_back(plane2);


            plane_h.push_back((h_mtr[i][j] + h_mtr[i + 1][j] + h_mtr[i + 1][j + 1]) / 3);
            plane_h.push_back((h_mtr[i][j] + h_mtr[i][j + 1] + h_mtr[i + 1][j + 1]) / 3);
        };
}


vector<Plane> Render::get_planes()
{
    return this->plain_model;
}


void Render::render_plane(const Plane &screenPlane, double maxHeight, double minHeight, double plane_real_h,
                          double intensity, double water_level)
{
    double yMin = screenPlane.getPMin().y();
    double yMax = screenPlane.getPMax().y();
    double a = screenPlane.getA(), c = screenPlane.getC();

    // cout << screenPlane.getP1().z() << " " << screenPlane.getP2().z() << " " << screenPlane.getP3().z() << '\n';

    QColor plane_color = _getColorByHeight(255, plane_real_h - water_level, maxHeight, minHeight);

    get_correct_color(plane_color, intensity);

    vector<Pixel_buff> line1 = this->_getLineByBresenham(screenPlane.getP1(), screenPlane.getP2());
    vector<Pixel_buff> line2 = this->_getLineByBresenham(screenPlane.getP2(), screenPlane.getP3());
    vector<Pixel_buff> line3 = this->_getLineByBresenham(screenPlane.getP3(), screenPlane.getP1());

    this->_calcParamsLine(line1, screenPlane.getP1().z(), screenPlane.getP2().z());
    this->_calcParamsLine(line2, screenPlane.getP2().z(), screenPlane.getP3().z());
    this->_calcParamsLine(line3, screenPlane.getP3().z(), screenPlane.getP1().z());

    vector<Pixel_buff> allLines;
    allLines.insert(allLines.end(), line1.begin(), line1.end());
    allLines.insert(allLines.end(), line2.begin(), line2.end());
    allLines.insert(allLines.end(), line3.begin(), line3.end());

    for (int y = yMin; y <= yMax; ++y)
    {
        vector<Pixel_buff> yn;
        std::copy_if(allLines.begin(), allLines.end(), std::back_inserter(yn), [y](const Pixel_buff &point)
        {
            return abs(point.vec.y() - y) < 0.01;
        });


        std::sort(yn.begin(), yn.end(), [](const Pixel_buff &a, const Pixel_buff &b)
        {
            return a.vec.x() < b.vec.x();
        });

        double deltaX = yn[yn.size() - 1].vec.x() - yn[0].vec.x();
        double x0 = yn[0].vec.x();

        double z = screenPlane.calcZ(x0, y);

        double I = yn[0].I, deltaI = I - yn[yn.size() - 1].I;
        double height = yn[0].vec.z(), deltaZ = height - yn[yn.size() - 1].vec.z();

        for (int x = x0; x <= yn[yn.size() - 1].vec.x(); ++x)
        {
            int sceneX = x, sceneY = y;

            if (sceneX < 0 || sceneX >= this->screenWidth || sceneY < 0 || sceneY >= this->screenHeight)
                continue;

            double deltaU = (x - x0) / deltaX - 0;

            if (z > this->zbuffer[sceneX][sceneY])
            {
                this->zbuffer[sceneX][sceneY] = z;

                this->framebuffer.setPixelColor(sceneX, sceneY, plane_color);
            }

            z -= (a / c);
            I += (deltaI * deltaU);
            height += (deltaZ * deltaU);
        }
    }
}

vector<Pixel_buff> Render::_getLineByBresenham(const QVector3D &p1, const QVector3D &p2)
{
    vector<Pixel_buff> result;

    int x1 = p1.x(), x2 = p2.x();
    int y1 = p1.y(), y2 = p2.y();

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    const int sx = x1 < x2 ? 1 : -1;
    const int sy = y1 < y2 ? 1 : -1;

    int error = dx - dy;

    while (x1 != x2 || y1 != y2)
    {
        result.push_back({QVector3D(x1, y1, 0), 0});

        int error2 = error * 2;
        if (error2 > -dy)
        {
            error -= dy;
            x1 += sx;
        }
        if (error2 < dx)
        {
            error += dx;
            y1 += sy;
        }
    }

    result.push_back({QVector3D(x2, y2, 0), 0});

    return result;
}

int Render::_getCorrectChannel(int channel, double I)
{
    int value = channel * I;

    value = (value > 255) ? 255 : value;
    value = (value < 0) ? 0 : value;

    return value;
}

void Render::get_correct_color(QColor &color, double I)
{
    color.setRed(_getCorrectChannel(color.red(), I));
    color.setGreen(_getCorrectChannel(color.green(), I));
    color.setBlue(_getCorrectChannel(color.blue(), I));
}

QColor Render::_getColorByHeight(double I, double height, double maxHeight, double waterlevel)
{
    return Color::get_color_by_height(height, maxHeight, waterlevel);
}


void Render::_calcParamsLine(vector<Pixel_buff> &line,
                             const double &ZPStart, const double &ZPEnd)
{
    int lineSize = line.size();

    //#pragma omp parallel for
    for (int i = 0; i < lineSize; ++i)
    {
        // длина, она же  AQ/AB из Роджерса
        double u = (double) (i + 1) / lineSize;
        // устанавливаем значение интенсивности в текущей точке на ребре
        //line[i].I = u * IPStart + (1 - u) * IPEnd;
        line[i].vec.setZ(u * ZPStart + (1 - u) * ZPEnd);
    }
}

void Render::render_terrain(const Terrain &terrain, QGraphicsScene *scene)
{
    this->clean();
    this->get_planes_model(terrain);

    double water_level = terrain.get_water_level();

    auto &intensity = terrain.get_intensity_matrix();

    for (int i = 0; i < plain_model.size(); ++i)
    {
        const Plane &plane = plain_model[i];
        render_plane(plane, terrain.get_max_h(), terrain.get_min_h(), plane_h[i], intensity[i], water_level);
    }

    //scene->addLine()

    scene->addPixmap(QPixmap::fromImage(framebuffer));
}

void Render::clean()
{
    for (int i = 0; i < this->screenHeight; ++i)
        zbuffer[i].assign(this->screenWidth, INT_MIN);

    this->framebuffer.fill(QColor(0, 0, 0));
    this->plain_model.clear();
    this->plane_h.clear();
}
