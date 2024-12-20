//
// Created by paa04 on 12.11.24.
//

#include "Terrain_manager.h"

#include <lightManager.h>
#include <Transform.h>

#include "Color.h"
#include "Diamond_Square.h"
#include "Fixed_Avg.h"
#include "Plane.h"
#include "_math.h"
#include "Terrain.h"

void Terrain_manager::gen_height_map(size_t map_n, Terrain &terrain, double smooth)
{
    Diamond_Square alg(map_n, smooth, make_shared<Fixed_Avg>());
    terrain = alg.generate_terrain();
    Diamond_Square::smoothHeightMap(terrain.get_height_matrix());
}

void Terrain_manager::cacl_screen_h_map(Terrain &terrain, int screen_wight, int screen_height, int max_height)
{
    auto &c_v = terrain.get_v_corrector();
    c_v = {0, 0, 1};

    Matrix<QVector3D> &mtr = terrain.get_screen_matrix();

    double ter_min_h = terrain.get_min_h();

    float dx = static_cast<float>(screen_wight) / (terrain.size() - 1);
    float dy = static_cast<float>(screen_height) / (terrain.size() - 1);

    for (int i = 0; i < terrain.size(); ++i)
    {
        for (int j = 0; j < terrain.size(); ++j)
        {
            mtr[i][j] = QVector3D{
                i * dx, j * dy, static_cast<float>((terrain[i][j] - ter_min_h) * max_height)
            };
        }
    }

    float cx = mtr.rbegin()->rbegin()->x() / 2;
    float cy = mtr.rbegin()->rbegin()->y() / 2;
    float cz = (terrain.get_max_h() - ter_min_h) * max_height / 2;

    terrain.set_center({cx, cy, cz});
}

void Terrain_manager::calcNormalForEachPlane(Terrain &terrain)
{
    int width = terrain.size();
    int lenght = terrain.size();

    Matrix<QVector3D> &screenHeightMap = terrain.get_screen_matrix();

    vector<QVector3D> &normalMap = terrain.get_normal_matrix();
    normalMap.clear();

    auto &corrector = terrain.get_v_corrector();
    corrector.normalize();

    for (int i = 0; i < width - 1; ++i)
        for (int j = 0; j < lenght - 1; ++j)
        {
            // в каждом квадрате сетки 2 треугольника - 2 плоскости
            Plane plane1(screenHeightMap[i][j], screenHeightMap[i + 1][j], screenHeightMap[i + 1][j + 1]);
            Plane plane2(screenHeightMap[i][j], screenHeightMap[i][j + 1], screenHeightMap[i + 1][j + 1]);

            // получаем вектора внешних нормалей к граням
            QVector3D normalPlane1(plane1.getA(), plane1.getB(), plane1.getC());
            QVector3D normalPlane2(plane2.getA(), plane2.getB(), plane2.getC());

            normalPlane1.normalize();
            normalPlane2.normalize();

            correct_normal(corrector, normalPlane1);
            correct_normal(corrector, normalPlane2);

            normalMap.push_back(normalPlane1);
            normalMap.push_back(normalPlane2);
        }

    //for (auto& i: normalMap)
    //   cout << i.x() << " " << i.y() << " " << i.z() << '\n';

    // cout << "----------\n";
}

void Terrain_manager::calcIntensityForEachPlane(Terrain &terrain, const Light &light, const Camera &camera)
{
    int rows = terrain.size();
    int cols = terrain.size();
    int normal_cnt = 0;

    terrain.intensity_clear();

    vector<double> &intensity = terrain.get_intensity_matrix();
    vector<QVector3D> &normal = terrain.get_normal_matrix();
    Matrix<QVector3D> &screenHeightMap = terrain.get_screen_matrix();

    const QVector3D &camera_vec = camera.get_vec();

    // цикл по всем вершинам ландшафтной сетки
    for (int i = 0; i < rows - 1; ++i)
        for (int j = 0; j < cols - 1; ++j)
        {
            Plane plane1(screenHeightMap[i][j], screenHeightMap[i + 1][j], screenHeightMap[i + 1][j + 1]);
            Plane plane2(screenHeightMap[i][j], screenHeightMap[i][j + 1], screenHeightMap[i + 1][j + 1]);
            // получили вектор направления света
            QVector3D direction1 = LightManager::caclDirectionVector(light, plane1.get_midP());
            QVector3D direction2 = LightManager::caclDirectionVector(light, plane2.get_midP());

            normal[normal_cnt].normalize();
            normal[normal_cnt + 1].normalize();


            if (QVector3D::dotProduct(-camera_vec, normal[normal_cnt]) >= 0)
                intensity.push_back(LightManager::caclIntensityAtPlane(light,
                                                                       direction1,
                                                                       normal[normal_cnt]));
            else
                intensity.push_back(0);

            if (QVector3D::dotProduct(-camera_vec, normal[normal_cnt]) >= 0)
                intensity.push_back(LightManager::caclIntensityAtPlane(light,
                                                                       direction2,
                                                                       normal[normal_cnt + 1]));
            else
                intensity.push_back(0);
            normal_cnt += 2;
        }

    //  for(auto& i : intensity)
    //   cout << i << "\n";
}


void Terrain_manager::viz_terrain(const Terrain &terrain, QGraphicsScene *scene)
{
    std::ofstream out;
    out.open("log_ter.txt");
    out << terrain;
    out.close();

    QImage im(terrain.size(), terrain.size(), QImage::Format_RGB32);

    for (int i = 0; i < terrain.size(); ++i)
        for (int j = 0; j < terrain.size(); ++j)
        {
            im.setPixelColor(i, j, Color::get_color_by_height(terrain[i][j], terrain.get_max_h(), terrain.get_min_h()));
        }

    scene->addPixmap(QPixmap::fromImage(im));
}

void Terrain_manager::update_terrain(Terrain &terrain, Light &light, const Camera &camera)
{
    calcNormalForEachPlane(terrain);
    calcIntensityForEachPlane(terrain, light, camera);
}


void Terrain_manager::rotate(Terrain &terain, const QVector3D &center, const rotate_axis &axis)
{
    Matrix<QVector3D> &screenMtr = terain.get_screen_matrix();

    auto &v_c = terain.get_v_corrector();
    Transform::rotate(v_c, {0, 0, 0}, axis);

    Transform::rotate(terain.get_center(), center, axis);

    for (auto &v: screenMtr)
        for (QVector3D &point: v)
        {
            Transform::rotate(point, center, axis);
        }
}

void Terrain_manager::move_terrain(Terrain &terrain, const move_data &vec)
{
    Matrix<QVector3D> &screenMtr = terrain.get_screen_matrix();

    Transform::move(terrain.get_center(), vec);

    for (auto &v: screenMtr)
        for (QVector3D &point: v)
        {
            Transform::move(point, vec);
        }
}

void Terrain_manager::scale_terrain(Terrain &terrain, const QVector3D &center, const scale_data &scale)
{
    Matrix<QVector3D> &screenMtr = terrain.get_screen_matrix();

    Transform::scale(terrain.get_center(), center, scale);

    auto &v_c = terrain.get_v_corrector();
    Transform::scale(v_c, {0, 0, 0}, scale);
    v_c.normalize();

    for (auto &v: screenMtr)
        for (QVector3D &point: v)
        {
            Transform::scale(point, center, scale);
        }
}

void Terrain_manager::save_height_map(const Terrain &terrain, const string &file_path)
{
    ofstream fout(file_path);

    const Matrix<double> &matrix_height = terrain.get_height_matrix();

    fout << matrix_height.size() << " ";

    for (auto &v: matrix_height)
    {
        for (auto &h: v)
        {
            fout << h << " ";
        }
    }

    fout.close();
}

void Terrain_manager::load_height_map(Terrain &terrain, const string &file_path)
{
    ifstream fin(file_path);

    terrain.clear();

    size_t mtr_size;
    fin >> mtr_size;

    terrain = Terrain(mtr_size);

    double max_h = INT_MIN, min_h = INT_MAX;

    Matrix<double> &matrix_height = terrain.get_height_matrix();

    for (auto &v: matrix_height)
    {
        for (auto &h: v)
        {
            fin >> h;
            max_h = max(max_h, h);
            min_h = min(min_h, h);
        }
    }

    terrain.set_max_h(max_h);
    terrain.set_min_h(min_h);

    fin.close();
}

void Terrain_manager::undo_single_op(Terrain &terrain)
{
    // terrain.undo_operation();
}
