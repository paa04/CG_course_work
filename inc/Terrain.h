//
// Created by paa04 on 30.10.24.
//

#ifndef TERRAIN_H
#define TERRAIN_H
#include <iosfwd>
#include <iostream>
#include <limits.h>
#include <vector>

#include "Plane.h"

using namespace std;

template<typename T>
using Matrix = vector<vector<T> >;

class Terrain
{
private:
    Matrix<double> height_mtr;
    Matrix<QVector3D> screen_mtr;

    vector<QVector3D> normal_plains;
    vector<double> intensity;

    double max_height;
    double min_height;

    double water_level = 0;

    QVector3D center_;

    QVector3D corrector_vector = {0, 0, 1};


public:
    explicit Terrain(): Terrain(0)
    {
    }

    explicit Terrain(size_t n): max_height(INT_MIN), min_height(INT_MAX)
    {
        height_mtr.resize(n);
        for (auto &v: height_mtr)
            v.resize(n);

        screen_mtr.resize(n);
        for (auto &v: screen_mtr)
            v.resize(n);
    }

    Matrix<QVector3D> &get_screen_matrix()
    {
        return screen_mtr;
    }

    const Matrix<QVector3D> &get_screen_matrix() const
    {
        return screen_mtr;
    }

    const Matrix<double> &get_height_matrix() const
    {
        return height_mtr;
    }

    Matrix<double> &get_height_matrix()
    {
        return height_mtr;
    }

    vector<QVector3D> &get_normal_matrix()
    {
        return normal_plains;
    }

    vector<double> &get_intensity_matrix()
    {
        return intensity;
    }

    const vector<double> &get_intensity_matrix() const
    {
        return intensity;
    }

    const vector<double> &operator[](size_t index) const
    {
        return height_mtr[index];
    }

    vector<double> &operator[](size_t index)
    {
        return height_mtr[index];
    }

    size_t size() const
    {
        return height_mtr.size();
    }

    double get_max_h() const
    {
        return max_height;
    }

    void set_max_h(double h)
    {
        max_height = h;
    }

    double get_min_h() const
    {
        return min_height;
    }

    void set_min_h(double h)
    {
        min_height = h;
    }

    void set_center(const QVector3D& vector_)
    {
        center_ = vector_;
    }

    QVector3D& get_center()
    {
        return center_;
    }

    void clear();

    void intensity_clear()
    {
        intensity.clear();
        intensity.resize(0);
    }

    QVector3D& get_v_corrector()
    {
        return corrector_vector;
    }

    double get_water_level() const
    {
        return water_level;
    }

    void set_water_level(double water_level)
    {
        this->water_level = water_level;
    }

};

std::ostream &operator<<(std::ostream &os, const Terrain &t);

#endif //TERRAIN_H
