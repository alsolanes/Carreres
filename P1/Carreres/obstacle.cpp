#include "obstacle.h"

Obstacle::Obstacle() : Objecte(NumVerticesF)
{

    // Genera un cub de dimensio 1.
    xorig = 0;
    x = 0.5;

    yorig = 0;
    y = 0.5;

    zorig = 0;
    z = 0.5;

    make();
}

void Obstacle::make() {
    Index = 0;

    vertices[0] = point4( xorig-x, yorig-y, zorig+z, 1.0 );
    vertices[1] = point4( xorig-x, yorig+y, zorig+z, 1.0 );
    vertices[2] = point4( xorig+x, yorig+y, zorig+z, 1.0 );
    vertices[3] = point4( xorig+x, yorig-y, zorig+z, 1.0 );
    vertices[4] = point4( xorig-x, yorig-y, zorig-z, 1.0 );
    vertices[5] = point4( xorig-x, yorig+y, zorig-z, 1.0 );
    vertices[6] = point4( xorig+x, yorig+y, zorig-z, 1.0 );
    vertices[7] = point4( xorig+x, yorig-y, zorig-z, 1.0 );

    // RGBA vertex_colorsr
    vertex_colors[0] =    color4( 0.0, 0.0, 1.0, 1.0 );  // black
    vertex_colors[1] =    color4( 1.0, 0.0, 0.0, 1.0 );  // red
    vertex_colors[2] =    color4( 1.0, 1.0, 0.0, 1.0 );  // yellow
    vertex_colors[3] =    color4( 0.0, 1.0, 0.0, 1.0 );  // green
    vertex_colors[4] =    color4( 0.0, 0.0, 1.0, 1.0 );  // blue
    vertex_colors[5] =    color4( 1.0, 0.0, 1.0, 1.0 );  // magenta
    vertex_colors[6] =    color4( 1.0, 1.0, 1.0, 1.0 );  // white
    vertex_colors[7] =    color4( 0.0, 1.0, 1.0, 1.0 );  // cyan


    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );

}

void Obstacle::quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;

}
