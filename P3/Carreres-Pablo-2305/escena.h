#ifndef ESCENA_H
#define ESCENA_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>


#include <Common.h>

#include "conjuntllums.h"
#include "llum.h"
#include "camera.h"
#include "objecte.h"
#include "cotxe.h"
#include "terra.h"
#include "obstacle.h"

using namespace std;

class escena
{
public:
    escena(Camera *thirdp, Camera *pano);
    ~escena();

    void addObjecte(Objecte *objecte);
    void aplicaTG(mat4 m);

    void aplicaTGCentrat(mat4 m);
    void reset();

    void draw();
    void CapsaMinCont3DEscena();

    void notificaRotacio(int,int,int);
    bool comprova_colisio(Objecte *o);

    bool canMove(Objecte *o,int,int);

    vec4 getCentreCapsa(Capsa3D);
    /*----------------*/
    /* Camera methods */
    void actualitzaCameraPanoramica( bool clip);
    void resetCameraPanoramica();
    void actualitzaCameraThirdPerson();
    void iniLookAtCotxe();
    /*----------------*/

    // Light methods

    // Programa de la GPU
    QGLShaderProgram *program;

    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;

    // Objectes de l'escena: a modificar. Ara nomes té un objecte. Cal afegir un conjunt d'objectes
    Cotxe *cotxe;
    Terra *terra;
    //afegir conjunt de llums
    QList<Obstacle *> obstacles;
    Camera *cam_panoramica;
    Camera *cam_thirdperson;
    ConjuntLlums *llums;
};


#endif // ESCENA_H
