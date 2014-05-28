#include "llum.h"

Llum::Llum(QString id)
{
    this->activa = 1;
    this->posicioLlum = vec4(0,-6,0,0);

    this->Idifusa = vec3(0.6,0.6,0.6);
    this->Iambient = vec3(0.2,0.2,0.2);
    this->Iespecular = vec3(0.2,0.2,0.2);

    this->atenuacio = vec3(0.01,0.01,0.01);
    this->direccio = vec3(0,1,0);
    this->coef_a = 1.0;

    std::cout<<id.toStdString()<<std::endl;
    this->idLlum = "llum"+id;
    std::cout<<idLlum.toStdString()<<std::endl;
}

void Llum::toGPU(QGLShaderProgram *program){
     gl_IdLlum.activa = program->uniformLocation(idLlum+".activa");
     gl_IdLlum.posicio = program->uniformLocation(idLlum+".LightPosition");
     gl_IdLlum.Idifusa = program->uniformLocation(idLlum+".Idifusa");
     gl_IdLlum.Iespecular = program->uniformLocation(idLlum+".Iespecular");
     gl_IdLlum.Iambient = program->uniformLocation(idLlum+".Iambient");
     gl_IdLlum.atenuacio = program->uniformLocation(idLlum+".atenuacio");
     gl_IdLlum.direccio = program->uniformLocation(idLlum+".direccio");
     gl_IdLlum.coef_a = program->uniformLocation(idLlum+".coef_a");

     // Bind de les zones de memoria que corresponen a les variables de la CPU
     glUniform1i(gl_IdLlum.activa, activa);
     glUniform4fv(gl_IdLlum.posicio, 1, posicioLlum); //posicioLlum és un vec4
     glUniform3fv(gl_IdLlum.Idifusa, 1, Idifusa); // difusa és un vec3
     glUniform3fv(gl_IdLlum.Iespecular, 1, Iespecular ); // especular és un vec3
     glUniform3fv(gl_IdLlum.Iambient, 1, Iambient ); // ambient és un vec3
     glUniform3fv(gl_IdLlum.atenuacio, 1, atenuacio ); // atenuacio és un vec3
     glUniform3fv(gl_IdLlum.direccio, 1, direccio ); // posicio és un vec3
     glUniform1f(gl_IdLlum.coef_a, coef_a); // coef_a és un Glfloat

}
