//
// Created by paa04 on 10.12.24.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <QVector3D>


class Camera {
    QVector3D _vec;

public:
    Camera();

    QVector3D& get_vec();
    [[nodiscard]] const QVector3D& get_vec() const;
    void set_vec(QVector3D& vec);
};



#endif //CAMERA_H
