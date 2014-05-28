#include <escena.h>

using namespace std;

escena::escena(Camera *pano, Camera *thirdp)

{
    CapsaMinCont3DEscena();

    cotxe = NULL;
    terra = NULL;

    /*// Modifiquem els valors de la camara
    pano->vs.angx = 30;
    pano->vs.angy = 30;
    pano->vs.angz = 30;
    pano->vs.obs = vec4(0,0,0,0);
    pano->vs.vrp = vec4(2,2,2,1);
    pano->piram.proj = PERSPECTIVA;
    pano->piram.d = 0;
    pano->piram.alfav = 0;
    pano->piram.alfah = 0;
    pano->piram.dant = 0;
    pano->piram.dpost = 0;
    */



    this->cam_panoramica = pano;
    this->cam_thirdperson = thirdp;
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
    capsaMinima.a = 10;
    capsaMinima.h = 10;
    capsaMinima.p = 10;
    capsaMinima.pmin = vec3(-5.0,-5.0,-5.0);
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
    return 1;
}

// Metodes de camara
/**
 * @brief escena::actualitzaCameraPanoramica
 * @param program
 * @param clip
 * S'encarrega de donar valors a tots els atributs de la càmera panoràmica relacionats amb la visualització.
 * Clip indica si l'escena s'està veient sencera o no(clipping, si fals -> tota l'escena visible per l'observador).
 */
void escena::actualitzaCameraPanoramica(bool clip){
    if(clip) {
      cam_panoramica->CalculWindowAmbRetallat();
    }
    cam_panoramica->CalculaMatriuModelView();
    cam_panoramica->CalculaMatriuProjection();
}

/**
 * @brief escena::resetCameraPanoramica
 * Situa la càmera panoràmica a la situació original(inicialment vista top i observador mirant al centre de l'escena).
 */
void escena::resetCameraPanoramica(){
    Camera *c = this->cam_panoramica;
    CapsaMinCont3DEscena();
    Capsa3D cap = capsaMinima;

    c->piram.proj=PARALLELA;
    c->piram.d=sqrt(cap.a*cap.a+cap.h*cap.h+cap.p*cap.p);
    c->piram.dant = c->piram.d*0.33f;
    c->piram.dpost = (c->piram.d)+capsaMinima.p;
    c->vs.angx = -90;
    c->vs.angy = 0;
    c->vs.vrp = getCentreCapsa(cap);
    c->vs.obs = c->CalculObs(getCentreCapsa(cap),c->piram.d,c->vs.angx,c->vs.angy);


}

/**
 * @brief escena::actualitzaCameraThirdPerson
 * @param program
 * Actualitza la càmera en 3era persona quan es mou el cotxe.
 */
void escena::actualitzaCameraThirdPerson(){
    if (cotxe == NULL) return;

    Camera *c = this->cam_thirdperson;
    Capsa3D capsa = cotxe->calculCapsa3D();
    c->vs.angy = cotxe->getYRot()+90;
    c->vs.vrp = getCentreCapsa(capsa);
    c->vs.obs = c->CalculObs(getCentreCapsa(capsa),c->piram.d,c->vs.angx,c->vs.angy);

}

/**
 * @brief escena::iniLookAtCotxe
 * Inicialitza la càmera en 3era persona(està mirant cap a la part davantera del cotxe des d'una posició del darrere
 * del cotxe i una mica elevada centrada en el cotxe i part reduida de l'escena).
 */
void escena::iniLookAtCotxe(){
    Capsa3D  capsa= cotxe->capsa;
    Camera * c = cam_thirdperson;
    c->piram.d = capsa.p*3.0f;
    c->piram.dant = c->piram.d*0.33f;
    c->piram.dpost = (c->piram.d)+capsa.p;
    c->vs.angx = -50;
    c->vs.angy = cotxe->getYRot()+90;
    c->vs.vrp = getCentreCapsa(capsa);
    c->vs.obs = c->CalculObs(getCentreCapsa(capsa),c->piram.d,c->vs.angx,c->vs.angy);
}

vec4 escena::getCentreCapsa(Capsa3D capsa){
    return vec4(capsa.pmin[0]+(0.5*capsa.a),capsa.pmin[1]+(0.5*capsa.h),capsa.pmin[2]+(0.5*capsa.p),1.0);
}

