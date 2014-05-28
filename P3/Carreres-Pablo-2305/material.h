#ifndef MATERIAL_H
#define MATERIAL_H
#include "Common.h"

class Material
{
public:
    Material();
    vec3 ks;
    vec3 ka;
    vec3 kd;
    float coef_a;
};

#endif // MATERIAL_H
