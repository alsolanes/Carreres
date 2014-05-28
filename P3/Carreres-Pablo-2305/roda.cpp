#include "roda.h"

Roda::Roda(int numVertex) : Objecte(numVertex)
{
    this->material = new Material();
    this->material->coef_a = 0.078125;
    this->material->ka = vec3(0.02, 	0.02, 	0.02);
    this->material->ks = vec3(0.4, 	0.4, 	0.4);
    this->material->kd = vec3(0.01, 	0.01, 	0.01);
    this->xRot = 0;
    this->yRot = 0;
    this->zRot = 0;
}
