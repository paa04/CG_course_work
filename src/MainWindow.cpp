//
// Created by paa04 on 11.11.24.
//

#include "MainWindow.h"
#include "MainWindow.h"

#include <Transform.h>
#include <QFileDialog>

#include "Color.h"
#include "Light_timer.h"
#include "Terrain_manager.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), timer_ptr(nullptr), terrain(7)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene);

    ui->graphicsView->setFixedSize(750, 750);
    ui->graphicsView->setSceneRect(0, 0, 750, 750);
    // ui->graphicsView->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->move_apply_btn, &QPushButton::pressed, this, &MainWindow::move_pressed);
    connect(ui->rotate_apply_btn, &QPushButton::pressed, this, &MainWindow::rotate_pressed);
    connect(ui->fill_with_center, &QPushButton::pressed, this, &MainWindow::fill_with_center);
    connect(ui->scale_apply_btn, &QPushButton::pressed, this, &MainWindow::scale_pressed);
    connect(ui->light_btn, &QPushButton::pressed, this, &MainWindow::set_light);
    connect(ui->generat_btn, &QPushButton::pressed, this, &MainWindow::generate_terrain);
    connect(ui->timer_btn, &QPushButton::pressed, this, &MainWindow::timer_pressed);
    connect(ui->undo_btn, &QPushButton::pressed, this, &MainWindow::undo_pressed);
    connect(ui->h_btn, &QPushButton::pressed, this, &MainWindow::h_pressed);
    connect(ui->water_btn, &QPushButton::pressed, this, &MainWindow::water_level_pressed);
    connect(ui->repeat_btn, &QPushButton::pressed, this, &MainWindow::repeat_pressed);

    QAction *save_action = ui->save_menu->addAction("save_action");
    QAction *load_action = ui->load_menu->addAction("load_action");

    connect(save_action, &QAction::triggered, this, &MainWindow::save_map);
    connect(load_action, &QAction::triggered, this, &MainWindow::open_map);

    //cout << ui->graphicsView->width() << '\n';

    // ui->graphicsView->setFixedSize(500, 500);
    // ui->graphicsView->setSceneRect(0, 0, 500, 500);
    //view->setFixedSize(width, height);
    // view->setSceneRect(0, 0, width, height);

    {
        auto pos = light.getPosition();
        ui->lightX->setValue(pos.x());
        ui->lightY->setValue(pos.y());
        ui->lightZ->setValue(pos.z());
    }

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::UpdateLight);

    // light = Light(QVector3D(0, ui->graphicsView->height(), 0), 2.5, 0.5);
    //ui->graphicsView->scene()->setSceneRect(0, 0, ui->graphicsView->width() - 10, ui->graphicsView->height() - 10);

    init();
}

void MainWindow::timer_pressed()
{
    if (!timer->isActive())
        timer->start(1000);
    else
        stop_timer();
}

void MainWindow::UpdateLight()
{
    if (light_upd_thread_.joinable())
    {
        clock_t beg = clock();
        light_upd_thread_.join();
        beg = clock() - beg;
        cout << beg << '\n';
        render.render_terrain(terrain, ui->graphicsView->scene());
    }

    lock_guard<mutex> lock(terrain_mutex_);
    auto l = light.getPosition();
    Transform::rotate(l, terrain.get_center(), {0, 10, 00});
    light.setPosition(l);

    light_upd_thread_ = std::thread(&Terrain_manager::calcIntensityForEachPlane, std::ref(terrain), std::ref(light),
                                    std::ref(camera));

    // Terrain_manager::calcIntensityForEachPlane(terrain, light, camera);

    //    Terrain_manager::update_terrain(terrain, light);
    //cout << "End\n";
}

void MainWindow::stop_timer()
{
    if (light_upd_thread_.joinable())
        light_upd_thread_.join();

    timer->stop();
}

void MainWindow::init()
{
    render = Render(ui->graphicsView->width(), ui->graphicsView->height());

    Color::init();
}

void MainWindow::generate_terrain()
{
    clock_t t = clock();

    stop_timer();
    operations_history.clear();

    Terrain_manager::gen_height_map(ui->n_polyg->value(), terrain, ui->noice_box->value());
    Terrain_manager::cacl_screen_h_map(terrain, ui->graphicsView->width(), ui->graphicsView->height(), 300);

    rotate_axis ra = {-60, 0, 0};

    auto l = light.getPosition();

    // Transform::scale(l, terrain.get_center(), {0.7, 0.7, 0.7});
    Terrain_manager::scale_terrain(terrain, terrain.get_center(), {0.7, 0.7, 0.7});
    operations_history.insert_op(
        make_shared<scale_operation>(scale_data{0.7, 0.7, 0.7}, terrain.get_center(), false, l));
    Terrain_manager::rotate(terrain, terrain.get_center(), {0, 0, 45});
    operations_history.insert_op(
        make_shared<rotate_operation>(rotate_axis{0, 0, 45}, terrain.get_center(), false, l));
    Terrain_manager::rotate(terrain, terrain.get_center(), ra);
    operations_history.insert_op(make_shared<rotate_operation>(ra, terrain.get_center(), false, l));


    //Terrain_manager::move_terrain(terrain, {0, -150, 0});


    // Transform::rotate(l, terrain.get_center(), {0, 0, 45});
    // Transform::rotate(l, terrain.get_center(), ra);
    //  Transform::move(l, {0, -150, 0});

    light.setPosition(l);

    Terrain_manager::update_terrain(terrain, light, camera);
    render.render_terrain(terrain, ui->graphicsView->scene());

    t = clock() - t;

    cout << ui->n_polyg->value() << " " << t << endl;
}

void MainWindow::open_map()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Открыть карту",
                                                    "",
                                                    "Файлы карт (*.map);;");

    if (!filename.isEmpty())
    {
        const char *name = filename.toLocal8Bit().constData();

        Terrain_manager::load_height_map(terrain, name);

        Terrain_manager::cacl_screen_h_map(terrain, ui->graphicsView->width(), ui->graphicsView->height(), 300);

        operations_history.clear();

        QVector3D l = {0, 0, 999};
        rotate_axis ra = {-60, 0, 0};

        Terrain_manager::scale_terrain(terrain, terrain.get_center(), {0.7, 0.7, 0.7});
        operations_history.insert_op(
            make_shared<scale_operation>(scale_data{0.7, 0.7, 0.7}, terrain.get_center(), false, l));
        Terrain_manager::rotate(terrain, terrain.get_center(), {0, 0, 45});
        operations_history.insert_op(
            make_shared<rotate_operation>(rotate_axis{0, 0, 45}, terrain.get_center(), false, l));
        Terrain_manager::rotate(terrain, terrain.get_center(), ra);
        operations_history.insert_op(make_shared<rotate_operation>(ra, terrain.get_center(), false, l));

        Terrain_manager::update_terrain(terrain, light, camera);
        render.render_terrain(terrain, ui->graphicsView->scene());
    }
}

void MainWindow::save_map()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Сохранить карту",
                                                    "",
                                                    "Файлы карт (*.map);;");

    if (!filename.isEmpty()) {
        if (!filename.endsWith(".map", Qt::CaseInsensitive)) {
            filename += ".map";
        }

        const char *name = filename.toLocal8Bit().constData();

        Terrain_manager::save_height_map(terrain, name);
    }
}

void MainWindow::render_scene()
{
    Terrain_manager::gen_height_map(7, terrain, 0.4);
    Terrain_manager::cacl_screen_h_map(terrain, ui->graphicsView->width(), ui->graphicsView->height(), 300);

    rotate_axis ra = {-60, 0, 0};

    auto l = light.getPosition();

    Terrain_manager::scale_terrain(terrain, terrain.get_center(), {0.7, 0.7, 0.7});
    operations_history.insert_op(
        make_shared<scale_operation>(scale_data{0.7, 0.7, 0.7}, terrain.get_center(), false, l));
    Terrain_manager::rotate(terrain, terrain.get_center(), {0, 0, 45});
    operations_history.insert_op(
        make_shared<rotate_operation>(rotate_axis{0, 0, 45}, terrain.get_center(), false, l));
    Terrain_manager::rotate(terrain, terrain.get_center(), ra);
    operations_history.insert_op(make_shared<rotate_operation>(ra, terrain.get_center(), false, l));


    Terrain_manager::update_terrain(terrain, light, camera);
    render.render_terrain(terrain, ui->graphicsView->scene());
}

void MainWindow::move_pressed()
{
    stop_timer();

    double dx = ui->moveX->value();
    double dy = ui->moveY->value();
    double dz = ui->moveZ->value();

    bool isCamera = false;

    if (ui->camera_radio->isChecked())
    {
        isCamera = true;

        auto l = light.getPosition();
        Transform::move(l, {dx, dy, dz});
        light.setPosition(l);
    }

    operations_history.insert_op(make_shared<move_operation>(move_data{dx, dy, dz}, isCamera, light.getPosition()));


    Terrain_manager::move_terrain(terrain, {dx, dy, dz});
    //if (!isCamera)
    Terrain_manager::update_terrain(terrain, light, camera);
    render.render_terrain(terrain, ui->graphicsView->scene());
}

void MainWindow::rotate_pressed()
{
    stop_timer();

    double
            dx = ui->rotateX->value(),
            dy = ui->rotateY->value(),
            dz = ui->rotateZ->value();

    float
            cx = ui->cx->value(),
            cy = ui->cy->value(),
            cz = ui->cx->value();

    bool isCamera = false;

    if (ui->camera_radio->isChecked())
    {
        isCamera = true;
        auto l = light.getPosition();
        Transform::rotate(l, {cx, cy, cz}, {dx, dy, dz});
        light.setPosition(l);
    }

    operations_history.insert_op(
        make_shared<rotate_operation>(rotate_axis{dx, dy, dz}, QVector3D{cx, cy, cz}, isCamera, light.getPosition()));


    Terrain_manager::rotate(terrain, {cx, cy, cz}, {dx, dy, dz});
    // if (!isCamera)
    Terrain_manager::update_terrain(terrain, light, camera);
    render.render_terrain(terrain, ui->graphicsView->scene());
}

void MainWindow::scale_pressed()
{
    stop_timer();

    double
            dx = ui->scaleX->value(),
            dy = ui->scaleY->value(),
            dz = ui->scaleZ->value();

    float
            cx = ui->scaleCX->value(),
            cy = ui->scaleCY->value(),
            cz = ui->scaleCZ->value();

    bool isCamera = false;

    if (ui->camera_radio->isChecked())
    {
        isCamera = true;
        auto l = light.getPosition();
        Transform::scale(l, {cx, cy, cz}, {dx, dy, dz});
        light.setPosition(l);
    }

    operations_history.insert_op(
        make_shared<scale_operation>(scale_data{dx, dy, dz}, QVector3D{cx, cy, cz}, isCamera, light.getPosition()));


    Terrain_manager::scale_terrain(terrain, {cx, cy, cz}, {dx, dy, dz});
    // if (!isCamera)
    Terrain_manager::update_terrain(terrain, light, camera);
    render.render_terrain(terrain, ui->graphicsView->scene());
}

void MainWindow::fill_with_center()
{
    auto center = terrain.get_center();
    ui->cx->setValue(center.x());
    ui->cy->setValue(center.y());
    ui->cz->setValue(center.z());
}

void MainWindow::set_light()
{
    stop_timer();

    float
            dx = ui->lightX->value(),
            dy = ui->lightY->value(),
            dz = ui->lightZ->value();

    QVector3D pos = {dx, dy, dz};

    light.setPosition(pos);
    Terrain_manager::calcIntensityForEachPlane(terrain, light, camera);
    render.render_terrain(terrain, ui->graphicsView->scene());
}

void MainWindow::h_pressed()
{
    double h = ui->h_box->value();

    stop_timer();

    Terrain_manager::cacl_screen_h_map(terrain, ui->graphicsView->width(), ui->graphicsView->height(), h);

    restore_ops();

    Terrain_manager::update_terrain(terrain, light, camera);
    render.render_terrain(terrain, ui->graphicsView->scene());
}

void MainWindow::water_level_pressed()
{
    stop_timer();

    double w_l = ui->water_box->value() / 100.0 * (terrain.get_max_h() - terrain.get_min_h());
    terrain.set_water_level(w_l);

    render.render_terrain(terrain, ui->graphicsView->scene());
}

void MainWindow::restore_ops()
{
    auto v = operations_history.actual_ops();
    for (auto &op: v)
        op->do_op(terrain, light);
}

void MainWindow::undo_pressed()
{
    stop_timer();

    auto op = operations_history.restore_op();

    if (op != nullptr)
    {
        op.get()->undo(terrain, light);
        //   operations_history.pop_back();

        Terrain_manager::update_terrain(terrain, light, camera);
        render.render_terrain(terrain, ui->graphicsView->scene());
    }
}

void MainWindow::repeat_pressed()
{
    stop_timer();

    auto op = operations_history.repeat_op();

    if (op != nullptr)
    {
        op.get()->do_op(terrain, light);

        Terrain_manager::update_terrain(terrain, light, camera);
        render.render_terrain(terrain, ui->graphicsView->scene());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}
