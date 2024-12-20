cmake_minimum_required(VERSION 3.28)
project(CW)

set(CMAKE_CXX_STANDARD 23)

#QT INCLUSION
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)

QT6_WRAP_UI( UI_HEADERS inc/mainwindow.ui)

include_directories(inc)
include_directories(ui)

add_executable(CW src/main.cpp
        inc/Diamond_Square.h
        inc/Terrain.h
        src/Terrain.cpp
        src/Diamond_Square.cpp
        inc/Avg_Strategy.h
        inc/Fixed_Avg.h
        src/Fixed_Avg.cpp
        src/Fixed_Avg.cpp
        inc/Plane.h
        inc/Render.h
        src/Render.cpp
        inc/Plane.h
        src/Plane.cpp
        src/MainWindow.cpp
        inc/mainwindow.ui
        ${UI_HEADERS}
        inc/MainWindow.h
        inc/Terrain_manager.h
        src/Terrain_manager.cpp
        inc/Color.h
        src/Color.cpp
        inc/light.h
        src/light.cpp
        inc/lightManager.h
        src/lightManager.cpp
        src/Transform.cpp
        inc/Light_timer.h
        src/Light_timer.cpp
        inc/_math.h
        src/_math.cpp
        inc/Camera.h
        src/Camera.cpp
        inc/Operation.h
        src/Operation.cpp
        inc/Operation_history.h
        src/Operation_history.cpp
        inc/experement.h
        src/experement.cpp)

#link QT
target_link_libraries(CW PRIVATE Qt6::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CW)
endif()