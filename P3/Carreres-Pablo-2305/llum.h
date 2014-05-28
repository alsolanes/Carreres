#ifndef LLUM_H
#define LLUM_H

#include <Common.h>
#include <QGLShaderProgram>
#include <QString>
#include <iostream>
class Llum
{

struct {
    GLuint activa;
    GLuint posicio;
    GLuint Iambient;
    GLuint Idifusa;
    GLuint Iespecular;
    GLuint atenuacio;
    GLuint coef_a;
    GLuint direccio;
} gl_IdLlum;

public:
    Llum(QString);
    void toGPU(QGLShaderProgram * program);

    //vars
    int activa;
    QString idLlum;
    vec4 posicioLlum;
    vec3 ld;
    vec3 Idifusa;
    vec3 Iespecular;
    vec3 Iambient;
    vec3 atenuacio;
    vec3 direccio;

    float coef_a;
};

#endif // LLUM_H
