#ifndef COTXE_H
#define COTXE_H

#include <Common.h>
#include <objecte.h>
#include "roda.h"
#include "carroceria.h"

#include <iostream>

using namespace std;

class Cotxe: public Objecte
{
public:
    Cotxe(QString n);
    Cotxe(QString n, GLfloat tamanio, GLfloat x0, GLfloat y0, GLfloat z0,
          double girx, double giry, double girz,
          float xdir, float ydir, float zdir);
    void aplicaTG(mat4 m);
    void aplicaTGPoints(mat4 m);
    // Aplica una TG centrada en el punt central de la capsa de l'objecte a un objecte
    void aplicaTGCentrat(mat4 m);
    void toGPU(QGLShaderProgram *pr);
    void readObj(QString filename);
    void make();
    void draw();
    void notificaRotacio(int, int, int);
    Capsa3D calculCapsa3D();
    void aplicaTGRodes(mat4,int);
    void aplicaTGCentratEixDevanter(mat4 m);


    void forward();
    void backward();
    void turnleft();
    void turnright();
    point4 direction;

    QList<Roda *> rodes;

private:
    void moure2D(float,float,int);

    Carroceria *carrosseria;
    static const int NumVerticesF = 100000;
    static const int NumCares = 300000;


};

#endif // COTXE_H
