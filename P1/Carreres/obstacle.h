#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <Common.h>
#include <objecte.h>
#include <ctime>
#include <iostream>

class Obstacle : public Objecte
{
public:
    Obstacle();
    void make();
    void quad(int,int,int,int);
private:
    float x;
    float y;
    float z;
    static const int NumVerticesF = 36;
    point4 vertices[8]; // 8 vertexs del cub
    color4 vertex_colors[8]; // 8 colors RGBA associats a cada vertex

};

#endif // OBSTACLE_H
