#include "light.h"

Light::Light() :
    // пока что случайные значения для последних двух аргументов
    Light(QVector3D(0, 0, 1000), 2.5, 0.5)
{
}

Light::Light(const QVector3D &position, double I_0, double K_d) :
    _position(position), _I_0(I_0), _K_d(K_d)
{
}

Light::Light(const Light &other)
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;
}

Light::Light(Light &&other) noexcept
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;
}

Light &Light::operator=(const Light &other)
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;

    return *this;
}

Light &Light::operator=(Light &&other) noexcept
{
    this->_position = other._position;
    this->_I_0 = other._I_0;
    this->_K_d = other._K_d;

    return *this;
}
