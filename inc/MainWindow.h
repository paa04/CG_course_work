//
// Created by paa04 on 11.11.24.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include <memory>
#include <thread>

#include "Camera.h"
#include "light.h"
#include "Light_timer.h"
#include "Operation.h"
#include "Operation_history.h"
#include "Render.h"
#include "cmake-build-debug/ui_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class Form;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void render_scene();

    ~MainWindow();
private:
    Ui::MainWindow *ui;
    Render render;
    Light light;
    Camera camera;
    unique_ptr<Light_timer> timer_ptr;
    QTimer* timer;

    Terrain terrain;

    Operation_history operations_history;

    std::thread light_upd_thread_;
    mutex terrain_mutex_;

    void init();

    void stop_timer();

    void restore_ops();

private slots:
    void UpdateLight();

    void move_pressed();
    void rotate_pressed();
    void fill_with_center();
    void scale_pressed();

    void h_pressed();
    void water_level_pressed();

    void set_light();
    void generate_terrain();
    void timer_pressed();
    void open_map();
    void save_map();
    void undo_pressed();
    void repeat_pressed();
};


#endif //MAINWINDOW_H
