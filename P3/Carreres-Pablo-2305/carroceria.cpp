#include "carroceria.h"

Carroceria::Carroceria(int numVertex) : Objecte(numVertex)
{
    this->material = new Material();
    this->material->coef_a = 0.4;
    this->material->ka = vec3(0.24725,     0.1995,         0.0745);
    this->material->ks = vec3(0.628281, 	0.555802, 	0.366065);
    this->material->kd = vec3(0.75164, 	0.60648, 	0.22648);
    this->xRot = 0;
    this->yRot = 0;
    this->zRot = 0;
}
