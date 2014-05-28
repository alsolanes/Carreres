#ifndef CONJUNTLLUMS_H
#define CONJUNTLLUMS_H

#include "Common.h"
#include "llum.h"
#include <QGLShaderProgram>

class ConjuntLlums
{
public:
    ConjuntLlums();
    void toGPU(QGLShaderProgram *program);
    void setAmbientToGPU(QGLShaderProgram *program);
    QList<Llum*> llums;

    GLuint ambientGlobalGL;
    vec3 ambientGlobal;

};

#endif // CONJUNTLLUMS_H
