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


#include "objecte.h"
#include "cotxe.h"
#include "terra.h"
#include "obstacle.h"

using namespace std;

class escena
{
public:
    escena();
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

    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;

    // Objectes de l'escena: a modificar. Ara nomes té un objecte. Cal afegir un conjunt d'objectes
    Cotxe *cotxe;
    Terra *terra;
    QList<Obstacle *> obstacles;

};


#endif // ESCENA_H
