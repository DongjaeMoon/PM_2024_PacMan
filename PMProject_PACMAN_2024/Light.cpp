#include <GL/freeglut.h>

#include <iostream>

#include "Light.h"

Light::Light(float x, float y, float z, int L_ID) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    lightID = L_ID;
}

void Light::setAmbient(float r, float g, float b, float a) {
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
    ambient[3] = a;
}
void Light::setDiffuse(float r, float g, float b, float a) {
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;
}
void Light::setSpecular(float r, float g, float b, float a) {
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
}

Vector4f Light::getAmbient() const {
    return ambient;
}
Vector4f Light::getDiffuse() const {
    return diffuse;
}
Vector4f Light::getSpecular() const {
    return specular;
}

int Light::getID() const {
    return lightID;
}
void Light::draw() const {
    glEnable(lightID);
    GLfloat ambientArr[4] = { ambient[0], ambient[1], ambient[2], ambient[3] };
    GLfloat diffuseArr[4] = { diffuse[0], diffuse[1], diffuse[2], diffuse[3] };
    GLfloat specularArr[4] = { specular[0], specular[1], specular[2], specular[3] };
    GLfloat position[4] = { pos[0], pos[1], pos[2], 1.0f };

    glLightfv(lightID, GL_AMBIENT, ambientArr);
    glLightfv(lightID, GL_DIFFUSE, diffuseArr);
    glLightfv(lightID, GL_SPECULAR, specularArr);
    glLightfv(lightID, GL_POSITION, position);
}
