#include <iostream>

#include "Shape3D.h"

Shape3D::Shape3D() {
    center = Vector3f();
}

Shape3D::Shape3D(float x, float y, float z) {
    center = Vector3f(x, y, z);
}

void Shape3D::setCenter(float x, float y, float z) {
    center = Vector3f(x, y, z);
}

void Shape3D::setCenter(const Vector3f& c) {
    center = c;
}

Vector3f Shape3D::getCenter() const {
    return center;
}

void Shape3D::setVelocity(float x, float y, float z) {
    velocity = Vector3f(x, y, z);
}

void Shape3D::setVelocity(const Vector3f& v) {
    velocity = v;
}

Vector3f Shape3D::getVelocity() const {
    return velocity;
}

void Shape3D::setMTL(const Material& m) {
    mtl = m;
}

const Material& Shape3D::getMTL() const {
    return mtl;
}

void Shape3D::move() {
    center = center + velocity;
}
