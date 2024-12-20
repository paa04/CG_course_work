//
// Created by paa04 on 12.11.24.
//

#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H
#include <light.h>
#include <qgraphicsscene.h>
#include <QImage>
#include <Transform.h>
#include <string>

#include "Camera.h"
#include "Terrain.h"

class Operation;

using namespace std;

class Terrain;



class Terrain_manager
{
public:
    static void gen_height_map(size_t map_n, Terrain &terrain, double smooth);

    static void cacl_screen_h_map(Terrain &terrain, int screen_wight, int screen_height, int max_height);

    static void calcNormalForEachPlane(Terrain &terrain);

    static void calcIntensityForEachPlane(Terrain &terrain, const Light &light, const Camera& camera);

    static void viz_terrain(const Terrain &terrain, QGraphicsScene *scene);

    static void rotate(Terrain &terain, const QVector3D &center, const rotate_axis &axis);

    static void move_terrain(Terrain& terrain, const move_data& vec);

    static void scale_terrain(Terrain& terrain, const QVector3D &center, const scale_data& scale);

    static void update_terrain(Terrain &terrain, Light &light, const Camera &camera);

    static void save_height_map(const Terrain &terrain, const string& file_path);

    static void load_height_map(Terrain &terrain, const string& file_path);

    static void undo_single_op(Terrain& terrain);

};


#endif //TERRAIN_MANAGER_H
