#include "objecte.h"
#include "readfile.h"

Objecte::Objecte(int npoints, QObject *parent) : numPoints(npoints) ,
    QObject(parent)
{
    points = new point4[npoints];
    normal = new vec4[npoints];
}

Objecte::Objecte(int npoints, QString n, GLdouble tamanio, GLdouble x0, GLdouble y0, GLdouble z0, double girx, double giry, double girz) : numPoints(npoints)
{
    points = new point4[npoints];
    normal = new vec4[npoints];
    tam = tamanio;
    std::cout<<"Estic en el constructor parametritzat del cotxe\n";
    xorig = x0;
    yorig = y0;
    zorig = z0;

    xRot = girx;
    yRot = giry;
    zRot = girz;


    nom = n;
    Index = 0;

}


Objecte::~Objecte()
{
    delete points;
    delete normal;
}


Capsa3D Objecte::calculCapsa3D()
{

    float miny,maxy,minx,maxx,minz,maxz;
    // Metode a implementar: calcula la capsa mínima contenidora d'un objecte
    minx = points[0].x;
    maxx = points[0].x;

    miny = points[0].y;
    maxy = points[0].y;

    minz = points[0].z;
    maxz = points[0].z;

    for(int i = 0; i<Index; i++){

        minx = std::min(minx,points[i].x);
        maxx = std::max(maxx,points[i].x);

        miny = std::min(miny,points[i].y);
        maxy = std::max(maxy,points[i].y);

        minz = std::min(minz,points[i].z);
        maxz = std::max(maxz,points[i].z);
    }

    capsa.pmin = vec3(minx,miny,minz);
    capsa.a = maxx-minx;
    capsa.h = maxy-miny;
    capsa.p = maxz-minz;

    return capsa;
}




void Objecte::aplicaTG(mat4 m)
{
    aplicaTGPoints(m);

    // Actualitzacio del vertex array per a preparar per pintar
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index,
                     &points[0] );

    calculCapsa3D();
}


void Objecte::aplicaTGPoints(mat4 m)
{
    point4  *transformed_points = new point4[Index];

    for ( int i = 0; i < Index; ++i ) {
        transformed_points[i] = m * points[i];
    }

    transformed_points = &transformed_points[0];
    points = &points[0];

    for ( int i = 0; i < Index; ++i )
    {
        // AQUI HAURIEM DE RECALCULAR LES NORMALS PER A CADA VERTEX
        points[i] = transformed_points[i];
        normal[i] = normalize(m*normal[i]);
    }

    delete transformed_points;
}

void Objecte::notificaRotacio(int x,int y, int z)
{

    this->xRot = (xRot+x)%360;
    this->yRot = (yRot+y)%360;
    this->zRot = (zRot+z)%360;
}

void Objecte::aplicaTGCentrat(mat4 m)
{
    // Desfes tots els canvis aplicats al objecte
    Capsa3D c = calculCapsa3D();
    GLfloat cx = c.pmin.x+(0.5*c.a);
    GLfloat cy = c.pmin.y+(0.5*c.h);
    GLfloat cz = c.pmin.z+(0.5*c.p);
    // Angles

    // Refes les rotacions


    mat4 ch = Translate(cx,cy,cz)*RotateZ(zRot)*RotateY(yRot)*RotateX(xRot);
    // Aplica la matriu
    ch *=m;

    // desfes les rotacions
    ch *= RotateX(-xRot)*RotateY(-yRot)*RotateZ(-zRot)*Translate(-cx,-cy,-cz);


    // Metode a modificar
    aplicaTG(ch);

}

void Objecte::toGPU(QGLShaderProgram *pr){

    program = pr;

    std::cout<<"Passo les dades de l'objecte a la GPU\n";

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * Index + sizeof(color4) * Index,
                  NULL, GL_STATIC_DRAW );
    program->link();

    program->bind();
    glEnable( GL_DEPTH_TEST );
}

// Pintat en la GPU.
void Objecte::draw()
{

    // cal activar el buffer de l'objecte. Potser que ja n'hi hagi un altre actiu
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // per si han canviat les coordenades dels punts
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * Index, sizeof(color4) * Index, &normal[0] );

    // Per a conservar el buffer
    int vertexLocation = program->attributeLocation("vPosition");
    int colorLocation = program->attributeLocation("vNormal");

    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);

    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(point4) * Index, 4);

    // Envia materials
    GLuint ks= program->uniformLocation("m.ks");
    GLuint ka = program->uniformLocation("m.ka");
    GLuint kd = program->uniformLocation("m.kd");
    GLuint coef_a = program->uniformLocation("m.coef_a");

    glUniform3fv(ks, 1, material->ks ); // ambient és un vec3
    glUniform3fv(ka, 1, material->ka ); // atenuacio és un vec3
    glUniform3fv(kd, 1, material->kd ); // posicio és un vec3
    glUniform1f(coef_a, material->coef_a); // coef_a és un Glfloat

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    // Abans nomes es feia: glDrawArrays( GL_TRIANGLES, 0, NumVerticesP );
}

void Objecte::make()
{
    // Recorregut de totes les care    draw();s per a posar-les en les estructures de la GPU
    // Cal recorrer l'estructura de l'objecte per a pintar les seves cares

    Index = 0;

    for(unsigned int i=0; i<cares.size(); i++)
    {

        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++)
        {

            points[Index] = vertexs[cares[i].idxVertices[j]];
            cares[i].calculaNormal(vertexs);
            normal[Index] = cares[i].normal;

            Index++;
        }
    }
}

float Objecte::getYOrig() {
    return this->yorig;
}

// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), tambe es llegeixen.
//  Tots els elements del fitxer es llegeixen com a un unic objecte.
//

void Objecte::readObj(QString filename)
{
    //Do nothing
}


void Objecte::construeix_cara ( char **words, int nwords, Objecte*objActual, int vindexUlt)
{
    Cara face;
    for (int i = 0; i < nwords; i++)
    {
        int numvertex;

        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9'))
        {
            ReadFile::get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.idxVertices.push_back(vindex - 1 - vindexUlt);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.idxVertices.push_back(objActual->vertexs.size() + vindex - vindexUlt);
            else
            {
                fprintf (stderr, "Zero indices not allowed");//: '%s'\n", str_orig);
                exit (-1);
            }
        }
    }
    objActual->cares.push_back(face);
}

bool Objecte::comprova_colisio(Objecte *o)
{
    calculCapsa3D();
    int axmax, aymax, azmax, bxmax, bymax, bzmax;
    axmax=o->capsa.pmin.x+o->capsa.a;
    aymax=o->capsa.pmin.y+o->capsa.h;
    azmax=o->capsa.pmin.z+o->capsa.p;
    bxmax=this->capsa.pmin.x+this->capsa.a;
    bymax=this->capsa.pmin.y+this->capsa.h;
    bzmax=this->capsa.pmin.z+this->capsa.p;
    return(axmax>=this->capsa.pmin.x && o->capsa.pmin.x<=bxmax)
            && (aymax>=this->capsa.pmin.y && o->capsa.pmin.y<=bymax)
            && (azmax>=this->capsa.pmin.z && o->capsa.pmin.z<=bzmax);
}
