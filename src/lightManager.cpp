#include "../inc/lightManager.h"

QVector3D LightManager::caclDirectionVector(const Light &light, const QVector3D &vertex)
{
    QVector3D light_position(light.getPosition());

    // вектор от точки до источника
    double xDirection = light_position.x() - vertex.x();
    double yDirection = light_position.y() - vertex.y();
    double zDirection = light_position.z() - vertex.z();

    QVector3D vec = QVector3D(xDirection, yDirection, zDirection);
    vec.normalize();

    return vec;
}

double LightManager::caclIntensityAtPlane(const Light &light,
                                          const QVector3D &direction,
                                          const QVector3D &normal)
{
    double _scalar_product = QVector3D::dotProduct(direction, normal);
    // согласно с формулой для модели освещения Ламберта
    double I = light.getI_0() * light.getK_d() * std::max(0.0, _scalar_product);


    return I;
}
