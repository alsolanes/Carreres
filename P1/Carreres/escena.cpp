#include <escena.h>

using namespace std;

escena::escena()

{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0; capsaMinima.pmin[1] = 0; capsaMinima.pmin[2]=0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

    cotxe = NULL;
    terra = NULL;
    //obstacles = new QList<Obstacle *>();
}


escena::~escena()
{
    printf("DESTRUCTOR!");

}

void escena::addObjecte(Objecte *objecte) {
    if (dynamic_cast<Cotxe*>(objecte))
        this->cotxe = (Cotxe*)objecte;
    else if (dynamic_cast<Terra*>(objecte))
        this->terra = (Terra*)objecte;
    else if (dynamic_cast<Obstacle*>(objecte))
        obstacles.append((Obstacle*)objecte);
}


void escena::CapsaMinCont3DEscena()
{
    // Metode a implementar
}

void escena::aplicaTG(mat4 m) {

    // Metode a modificar

    if (cotxe!=NULL)
        cotxe->aplicaTG(m);
    if (terra!=NULL)
        terra->aplicaTG(m);
    for (int i = 0; i<obstacles.size();i++)
        obstacles[i]->aplicaTG(m);
}
void escena::notificaRotacio(int x, int y, int z)
{

    if (cotxe!=NULL)
        cotxe->notificaRotacio(x,y,z);
    if (terra!=NULL)
        terra->notificaRotacio(x,y,z);
    for (int i = 0; i<obstacles.size();i++)
        obstacles[i]->notificaRotacio(x,y,z);
}

void escena::aplicaTGCentrat(mat4 m) {

    // Metode a modificar

    if (cotxe!=NULL)
        cotxe->aplicaTGCentrat(m);
    if (terra!=NULL)
        terra->aplicaTGCentrat(m);
    for (int i = 0; i<obstacles.size();i++)
        obstacles[i]->aplicaTGCentrat(m);
}

void escena::draw() {

    // Metode a modificar

    if (cotxe!=NULL)
        cotxe->draw();
    if (terra!=NULL)
        terra->draw();
    for (int i = 0; i<obstacles.size();i++)
        obstacles[i]->draw();
}

void escena::reset() {

    // Metode a modificar

    if (cotxe!=NULL)
        cotxe->make();
    if (terra!=NULL)
        terra->make();
    for (int i = 0; i<obstacles.size();i++)
        obstacles[i]->make();

}

bool escena::comprova_colisio(Objecte *o){
    for(int i = 0; i<obstacles.size();i++){
        if(obstacles[i]->comprova_colisio(o)) return true;
    }
    //if(cotxe->comprova_colisio(o))return true;
    return false;
}

bool escena::canMove(Objecte *o, int angle, int modul){
    Capsa3D c = o->calculCapsa3D();
    Objecte *tmp = new Objecte(0);
    float radangle = angle*(3.14159265359/180.0);
    c.pmin.x += modul*std::cos(radangle);
    c.pmin.z += -modul*std::sin(radangle);
    tmp->capsa = c;
    return !comprova_colisio(tmp);
}

