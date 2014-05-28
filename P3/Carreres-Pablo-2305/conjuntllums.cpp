#include "conjuntllums.h"

ConjuntLlums::ConjuntLlums()
{
    ambientGlobal = vec3(1.0,1.0,1.0);
}

// Envia les dades de les llums a la GPU
void ConjuntLlums::toGPU(QGLShaderProgram *program){
    for(int i = 0; i <llums.length(); i++){
        llums.at(i)->toGPU(program);
    }
}


// Passa les dades de les llums actives a GPU i llum ambient.
void ConjuntLlums::setAmbientToGPU(QGLShaderProgram *program){
    ambientGlobalGL = program->uniformLocation("ambientGlobal");
    glUniform3fv(ambientGlobalGL, 1, ambientGlobal); // difusa és un vec3

    this->toGPU(program);

}
