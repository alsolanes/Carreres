#include "terra.h"

Terra::Terra(float x, float y, float z) : Objecte(NumVerticesF)
{
    this->material = new Material();
    this->material->coef_a = 0.6;
    this->material->ka = vec3(0.25, 	0.25, 	0.25);
    this->material->kd = vec3(0.4, 	0.4, 	0.4);
    this->material->ks = vec3(0.774597, 	0.774597, 	0.774597);


    printf("\nTerra creat!\n");
    this->x = x/2;
    this->z = z/2;
    this->y = y ;
    this->make();

}

void Terra::make(){
    static vec3  base_colors[] = {
        vec3( 1.0, 0.0, 0.0 ),
        vec3( 0.0, 1.0, 0.0 ),
        vec3( 0.0, 0.0, 1.0 ),
        vec3( 1.0, 1.0, 0.0 )
    };

    Index=6;
    this->points[0] = point4(-x, y,-z, 1.0);
    this->points[1] = point4( x, y,-z, 1.0);
    this->points[2] = point4(-x, y, z, 1.0);
    this->points[3] = point4(-x, y, z, 1.0);
    this->points[4] = point4( x, y,-z, 1.0);
    this->points[5] = point4( x, y, z, 1.0);
}
