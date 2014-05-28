#ifndef TERRA_H
#define TERRA_H

#include <Common.h>
#include <objecte.h>

#include <iostream>


class Terra : public Objecte
{
public:
    Terra(float,float,float);
    void make();

private:
    float x;
    float y;
    float z;
    static const int NumVerticesF = 6;
};

#endif // TERRA_H
