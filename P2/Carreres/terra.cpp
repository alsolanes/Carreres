#include "terra.h"

Terra::Terra(float x, float y, float z) : Objecte(NumVerticesF)
{
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
    this->points[0] = point4(-x, y,-z, 1.0); this->colors[0] = base_colors[0];  // red;
    this->points[1] = point4( x, y,-z, 1.0); this->colors[1] = base_colors[1];  // red;
    this->points[2] = point4(-x, y, z, 1.0); this->colors[2] = base_colors[2];  // red;
    this->points[3] = point4(-x, y, z, 1.0); this->colors[3] = base_colors[2];  // red;
    this->points[4] = point4( x, y,-z, 1.0); this->colors[4] = base_colors[1];  // red;
    this->points[5] = point4( x, y, z, 1.0); this->colors[5] = base_colors[3];  // red;
}
