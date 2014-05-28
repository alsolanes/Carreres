#include "cotxe.h"

Cotxe::Cotxe(QString n) : Objecte(NumVerticesF)
{
    nom = n;
    Index = 0;
    tam = 1;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    this->direction[0] = -1;
    this->direction[1] = 0;
    this->direction[2] = 0;
    this->direction[3] = 0;
    readObj(n);
    make();
}

Cotxe::Cotxe(QString n, GLfloat tamanio, GLfloat x0, GLfloat y0, GLfloat z0,
             double girx, double giry, double girz,
             float xdir, float ydir, float zdir):Objecte(NumVerticesF, n, tamanio, x0, y0, z0, girx, giry, girz)
{
    // El seguent codi escala el cotxe entre 0 i 1 i el situa el seu centre  0,0,0. aixo fa que es vegi en la primera visualització
    //
    // Cal modificar el codi seguent

    nom = n;
    Index = 0;
    tam = tamanio;

    xorig = x0;
    yorig = y0;
    zorig = z0;

    xRot = girx;
    yRot = giry;
    zRot = girz;

    this->direction[0] = xdir;
    this->direction[1] = ydir;
    this->direction[2] = zdir;
    this->direction[3] = 0;



    readObj(n);
    for(int i = 0; i < rodes.size();i++)
    {
        rodes.at(i)->xRot = xdir;
        rodes.at(i)->yRot = ydir;
        rodes.at(i)->zRot = zdir;
    }
    carrosseria->xRot = xdir;
    carrosseria->yRot = ydir;
    carrosseria->zRot = zdir;


    make();

}

// Overwritten method.
void Cotxe::aplicaTG(mat4 m){
    for(int i = 0; i<rodes.size();i++){
        rodes[i]->aplicaTG(m);
    }
    carrosseria->aplicaTG(m);
}

void Cotxe::aplicaTGRodes(mat4 m, int flag){ // 1 per totes, 0 per a les de devant
    if (flag){
        for(int i = 0; i<rodes.size();i++)
        {

            rodes[i]->aplicaTGCentrat(m);

        }
    return;
    }
    rodes[2]->aplicaTGCentrat(m);
    rodes[3]->aplicaTGCentrat(m);

}

// Overwritten method.
void Cotxe::aplicaTGPoints(mat4 m){
    for(int i = 0; i<rodes.size();i++){
        rodes[i]->aplicaTGPoints(m);
    }
    carrosseria->aplicaTGPoints(m);
}

// Overwritten method.
void Cotxe::aplicaTGCentrat(mat4 m){
    for(int i = 0; i<rodes.size();i++){
        rodes[i]->aplicaTGCentrat(m);
    }
    carrosseria->aplicaTGCentrat(m);
}

// Overwritten method.
void Cotxe::draw()
{
    for(int i = 0; i<rodes.size();i++){
        rodes[i]->draw();
    }
    carrosseria->draw();
}

// Overwritten method.
void Cotxe::toGPU(QGLShaderProgram *pr)
{
    for(int i = 0; i<rodes.size();i++){
        rodes[i]->toGPU(pr);
    }
    carrosseria->toGPU(pr);
}

// Overwritten method.
void Cotxe::make()
{

    for(int i = 0; i<rodes.size();i++){
        rodes[i]->make();
    }
    carrosseria->make();

    //Apliquem les matrius de transformacions per a les dades
    //introduides per l'usuari.
    // 1 - Escalat
    // 2 - Rotacio
    // 3 - Traslacio

    mat4 transform = Translate(xorig, yorig,zorig)*RotateX(direction[0])*RotateY(direction[1])*RotateZ(direction[2])*Scale(tam);



    calculCapsa3D();
    float centrex = capsa.pmin.x+capsa.a/2.0;
    float centrey = capsa.pmin.y+capsa.h/2.0;
    float centrez = capsa.pmin.z+capsa.p/2.0;
    float scale = std::max(capsa.a,capsa.h);
    scale = 1.0/(std::max(scale,capsa.p));
    transform *= Scale(scale)*Translate(-centrex,-centrey,-centrez);

    aplicaTG(transform);


}

// Overwritten method.
Capsa3D Cotxe::calculCapsa3D()
{

    GLfloat miny,maxy,minx,maxx,minz,maxz;

    // Init points;
    miny = rodes[0]->points[0].y;
    maxy = rodes[0]->points[0].y;
    minx = rodes[0]->points[0].x;
    maxx = rodes[0]->points[0].x;
    minz = rodes[0]->points[0].z;
    maxz = rodes[0]->points[0].z;

    // Comprova les rodes
    for(int i = 0; i<rodes.size();i++){
        for(int j = 0; j<rodes[i]->numPoints; j++){
            minx = std::min(minx,rodes[i]->points[j].x);
            maxx = std::max(maxx,rodes[i]->points[j].x);

            miny = std::min(miny,rodes[i]->points[j].y);
            maxy = std::max(maxy,rodes[i]->points[j].y);

            minz = std::min(minz,rodes[i]->points[j].z);
            maxz = std::max(maxz,rodes[i]->points[j].z);
        }

    }

    // Comprova la carrosseria
    for(int j = 0; j<carrosseria->numPoints; j++){
        minx = std::min(minx,carrosseria->points[j].x);
        maxx = std::max(maxx,carrosseria->points[j].x);

        miny = std::min(miny,carrosseria->points[j].y);
        maxy = std::max(maxy,carrosseria->points[j].y);

        minz = std::min(minz,carrosseria->points[j].z);
        maxz = std::max(maxz,carrosseria->points[j].z);
    }



    capsa.pmin = vec3(minx,miny,minz);
    capsa.a = maxx-minx;
    capsa.h = maxy-miny;
    capsa.p = maxz-minz;

    return capsa;
}




void Cotxe::readObj(QString filename)
{

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {
        Objecte *actual = NULL;
        int vindexAct = 0;
        int vindexUlt = 0;

        while (true)
        {
            //if (actual != NULL) actual->numPoints = vindexAct - vindexUlt;
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */

                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4)
                {
                    fprintf (stderr, "Too few coordinates");//: '%s'", str_orig);
                    exit (-1);
                }
                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5)
                {
                  QString sw(ReadFile::words[4]);
                  double w = sw.toDouble();
                  x/=w;
                  y/=w;
                  z/=w;
                }
                // S'afegeix el vertex a l'objecte
                actual->vertexs.push_back(point4(x, y, z, 1));
                vindexAct++;

            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'objecte
                construeix_cara (&ReadFile::words[1], nwords-1, actual, vindexUlt);

            }
            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            else if (!strcmp (first_word, "o")) {

                /*
                 * La forma en que creem els objectes
                 * ha de ser optimitzada. ja que el tamany de les
                 * llistes de vertex es creat de forma random
                 */


                //cada nou objecte s'actualitza aquest Ã­ndex
                //if (actual != NULL) actual->numPoints = vindexAct - vindexUlt;
                std::cout << ReadFile::words[1];
                if (!strcmp (ReadFile::words[1], "Roda_Esquerra_Posterior_Untitled")){
                    Roda *r = new Roda(30000);
                    rodes.append(r);
                    actual = r;
                }
                else if (!strcmp (ReadFile::words[1], "Roda_Dreta_Posterior_04")){
                    Roda *r = new Roda(30000);
                    rodes.append(r);
                    actual = r;
                }
                else if (!strcmp (ReadFile::words[1], "Roda_Esquerra_Davantera_02")){
                    Roda *r = new Roda(30000);
                    rodes.append(r);
                    actual = r;
                }
                else if (!strcmp (ReadFile::words[1], "Carrosseria_00")){
                    Carroceria *c = new Carroceria(100000);
                    carrosseria = c;
                    actual = c;
                }
                else if (!strcmp (ReadFile::words[1], "Roda_Dreta_Davantera_03")){
                    Roda *r = new Roda(30000);
                    rodes.append(r);
                    actual = r;
                }
                vindexUlt = vindexAct;
            }

            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }

            //free(words);
        }

    }

}


void Cotxe::notificaRotacio(int x, int y, int z)
{
    for(int i = 0; i<rodes.size(); i++)
        rodes.at(i)->notificaRotacio(x,y,z);
    carrosseria->notificaRotacio(x,y,z);
}



void Cotxe::forward(){
    moure2D(this->rodes.at(3)->yRot-180,0.05,1);

}

void Cotxe::backward(){
    moure2D(this->rodes.at(3)->yRot,0.05,0);
}


void Cotxe::aplicaTGCentratEixDevanter(mat4 m){
    Capsa3D c2 = rodes.at(2)->calculCapsa3D();
    Capsa3D c3 = rodes.at(3)->calculCapsa3D();

    vec3 cent2 = vec3((c2.pmin.x+c2.a)/2.0,(c2.pmin.y+c2.h)/2.0,(c2.pmin.z+c2.p)/2.0);
    vec3 cent3 = vec3((c3.pmin.x+c3.a)/2.0,(c3.pmin.y+c3.h)/2.0,(c3.pmin.z+c3.p)/2.0);

    GLfloat cx = (cent2.x+cent3.x)/2.0;
    GLfloat cy = (cent2.y+cent3.y)/2.0;
    GLfloat cz = (cent2.z+cent3.z)/2.0;

    mat4 trans = Translate(cx,cy,cz);
    mat4 deTrans = Translate(-cx,-cy,-cz);

    //Roda0
    for(int i = 0; i < rodes.size();i++){
        mat4 ch = trans*RotateZ(rodes.at(i)->zRot)*RotateY(rodes.at(i)->yRot)*RotateX(rodes.at(i)->xRot);
        ch *=m;
        ch *= RotateX(-rodes.at(i)->xRot)*RotateY(-rodes.at(i)->yRot)*RotateZ(-rodes.at(i)->zRot)*deTrans;
        rodes.at(i)->aplicaTG(ch);
    }
    //Carrosseria
    mat4 ch = Translate(cx,cy,cz)*RotateZ(carrosseria->zRot)*RotateY(carrosseria->yRot)*RotateX(carrosseria->xRot);
    ch *=m;
    ch *= RotateX(-carrosseria->xRot)*RotateY(-carrosseria->yRot)*RotateZ(-carrosseria->zRot)*deTrans;
    carrosseria->aplicaTG(ch);
}

void Cotxe::turnright(){
    // Metode a implementar per fer el moviment del cotxe
    if(std::abs(((rodes.at(2)->yRot+10)-carrosseria->yRot)%360)<50 || std::abs(((rodes.at(2)->yRot+10)-carrosseria->yRot)%360)>300){
        aplicaTGRodes(RotateY(10),0);
        rodes.at(2)->notificaRotacio(0,10,0);
        rodes.at(3)->notificaRotacio(0,10,0);
    }
}
void Cotxe::turnleft(){
    // Metode a implementar per fer el moviment del cotxe
    if(std::abs(((rodes.at(2)->yRot-10)-carrosseria->yRot)%360)<50 || std::abs(((rodes.at(2)->yRot-10)-carrosseria->yRot)%360)>300){

        aplicaTGRodes(RotateY(-10),0);
        rodes.at(2)->notificaRotacio(0,-10,0);
        rodes.at(3)->notificaRotacio(0,-10,0);

    }
}

void Cotxe::moure2D(float angle, float modul, int dir){
    //En aquesta funcio rebem el moviment com a un vector en forma polar.
    // Rotacio rodes
    if (angle == this->rodes.at(3)->yRot)
        aplicaTGRodes(RotateZ(10),1);
    else
        aplicaTGRodes(RotateZ(-10),1);
    //aplicaTG(Translate(-tam/200.0,0.0,0.0));

    // Traslladem el cotxe en la direccio de les rodes (si anem cap endevant)

    float radangle = angle*(3.14159265359/180.0);

    float x = modul*std::cos(radangle);
    float z = -modul*std::sin(radangle);


    if ((carrosseria->yRot%360)!=(rodes.at(3)->yRot%360) && dir){
        if (carrosseria->yRot== rodes.at(3)->yRot){}


        else if(rodes.at(3)->yRot>carrosseria->yRot && ((rodes.at(3)->yRot-carrosseria->yRot)%180)<90){

            aplicaTGCentratEixDevanter(RotateY(5));
            carrosseria->notificaRotacio(0,5,0);
            rodes.at(0)->notificaRotacio(0,5,0);
            rodes.at(1)->notificaRotacio(0,5,0);
            aplicaTGRodes(RotateY(-5),0);
            //rodes.at(2)->notificaRotacio(0,5,0);
            //rodes.at(3)->notificaRotacio(0,5,0);
        }
        else/*if ((carrosseria->yRot)-(rodes.at(3)->yRot) >   0)*/{

            aplicaTGCentratEixDevanter(RotateY(-5));
            carrosseria->notificaRotacio(0,-5,0);
            rodes.at(0)->notificaRotacio(0,-5,0);
            rodes.at(1)->notificaRotacio(0,-5,0);
            aplicaTGRodes(RotateY(5),0);

        }
    }

    aplicaTG(Translate(x,0.0,z));


}

int Cotxe::getYRot(){
    return carrosseria->yRot;
}
