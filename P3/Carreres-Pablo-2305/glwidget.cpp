#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include "llum.h"
#include "glwidget.h"
#include <QTime>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setFocusPolicy( Qt::StrongFocus );


    xRot = 0;
    yRot = 0;
    zRot = 0;

    a = 50.0;
    h = 50.0;
    p = 50.0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    program = 0;

    Camera * pano = new Camera();
    Camera * thirdp = new Camera();

    esc = new escena(pano, thirdp);


    pano->piram.proj = PARALLELA;
    pano->ini(this->size().width(),this->height(),esc->capsaMinima);
    thirdp->piram.proj = PERSPECTIVA;
    thirdp->ini(this->size().width(),this->height(),esc->capsaMinima);
    esc->llums = new ConjuntLlums();
    this->cameraActual = thirdp;

    // Creacio llums
    Llum *llum = new Llum("1");
    esc->llums->llums.append(llum);

    //llum2
    llum = new Llum("2");
    llum->activa = 0;
    esc->llums->llums.append(llum);

    //lum3
    llum = new Llum("3");
    llum->activa = 0;
    esc->llums->llums.append(llum);




}


GLWidget::~GLWidget()
{
    makeCurrent();
}

void GLWidget::InitShader( const char* vShaderFile, const char* fShaderFile)
{
    struct Shader {
        const char* filename;
        GLenum type;
        GLchar* source;
    }
    shaders[2] = {
        { vShaderFile, GL_VERTEX_SHADER, NULL },
        { fShaderFile, GL_FRAGMENT_SHADER, NULL }
    };
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    // Es llegeixen els dos shaders: el vertex i el fragment shader
    for ( int i = 0; i < 2; ++i ) {
        Shader& s = shaders[i];
        s.source = Common::readShaderSource( s.filename );
        if ( shaders[i].source == NULL ) {
            std::cerr << "Failed to read " << s.filename << std::endl;
            exit( EXIT_FAILURE );
        }
    }
    // Es compilen els programes en temps d'execució de l'aplicació

    vshader->compileSourceCode(shaders[0].source);
    fshader->compileSourceCode(shaders[1].source);

    // S'afegeixen a una variable de classe
    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    // Es munta el programa
    program->link();
    // Es vincula el programa al context de GL per a ser executat amb les comandes de GL
    program->bind();



}

// Metode per inicialitzar els shaders de l'aplicacio
void GLWidget::initShadersGPU()
{
    // Carrega dels shaders i posa a punt per utilitzar els programes carregats a la GPU
    InitShader( "../Carreres/vshader11.glsl", "../Carreres/fshader11.glsl" );
    //InitShader( program2, "../Carreres/vshader12.glsl", "../Carreres/fshader11.glsl" );
    //InitShader( program3, "../Carreres/vshader13.glsl", "../Carreres/fshader11.glsl" );

}



QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const

{
    return QSize(400, 400);
}

// Metodes per a poder rotar l'escena

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj) {

    // Per adaptar l'objecte suposem el tamany del widget sera de 50x50x50 amb centre en 0,0,0. Per tant
    // i tenint en compte que el tamany original es de 2x2x2 (entre -1,-1,-1, i 1,1,1) l'escalat l'haurem
    // de fer per 25.

    //obj->aplicaTG(scaleMatrix);

}

void GLWidget::newObjecte(Objecte * obj)
{
    adaptaObjecteTamanyWidget(obj);
    obj->toGPU(program);
    esc->addObjecte(obj);
    esc->llums->setAmbientToGPU(program);


    cameraActual->update(program);
    updateGL();
}

void GLWidget::newObstacle(int nombre)
{
    // Metode que serveix per a donar d'alta un obstacle amb el punt original a xorig, yorig,zorig
    // d'una certa mida
    // ha d'estar a les ys del pla de la terra

    // Metode a implementar
    for (int i = 0; i < nombre; i++){
        //bool flag = false;
        while(1/*!flag*/){
            Obstacle *o = new Obstacle();
            GLfloat xorig = (std::rand()%(int)(this->x/2))-(x/2);
            GLfloat zorig = (std::rand()%(int)(this->z/2))-(z/2);
            int escala = (std::rand()%5)+3;
            o->aplicaTG(Translate(xorig,y,zorig)*Scale(escala));
            o->calculCapsa3D();

            if(!esc->comprova_colisio(o))
            {
                newObjecte(o);
                break;
            }

        }

    }


}
void GLWidget::newTerra(float amplaria, float profunditat, float y)
{
    // Metode que crea un objecte terra poligon amb el punt original a xorig, yorig, zorig
    // (quadrat d'una certa mida amb origen a xorig, yorig, zorig

    this->x=amplaria;
    this->z=profunditat;
    this->y=y;
    // Metode a implementar
    Terra *t = new Terra(amplaria,y,profunditat);
    newObjecte(t);

 }

void GLWidget::newCotxe(QString fichero, float xorig, float zorig, float mida, float xdirec, float ydirec, float zdirec)
{
    Cotxe *obj;
    // parametres que posen l'objecte cotxe al punt original xorig, yorig, zorig i d'una certa mida
    float yorig = this->y;

    obj = new Cotxe(fichero, mida, xorig, yorig, zorig, 0., 0., 0.,xdirec, ydirec, zdirec);

    newObjecte(obj);

    if (cameraActual == esc->cam_thirdperson){
        esc->iniLookAtCotxe();
        cameraActual->update(program);
    }
}

void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    std::cout<<"Estic inicialitzant el shaders"<<std::ends;
    initShadersGPU();

    // A canviar per posar una imatge de fons: Estrelles...
    qglClearColor(qtPurple.dark());

}

void GLWidget::resetView()
{
    xRot = 0;
    yRot = 0;
    zRot = 0;


    esc->reset();

    updateGL();
}

void GLWidget::paintGL()
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    esc->draw();
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-150, +150, -150, +150, 0.0, 150.0);
#else
    glOrtho(-150, +150, -150, +150, 0.0, 150.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::setXRotation(int angle)
{
    angle = cameraActual->vs.angy + (angle);
    qNormalizeAngle(angle);
    cameraActual->modificaCameraAngle(0,angle);
    cameraActual->update(program);
    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    angle = cameraActual->vs.angx + (angle);
    qNormalizeAngle(angle);
    cameraActual->modificaCameraAngle(angle,0);
    cameraActual->update(program);
    updateGL();
}


void GLWidget::Zoom(int positiu)
{
    cameraActual->zoom(positiu);
    cameraActual->update(program);
    updateGL();
}

void GLWidget::Pan(int dx, int dy)
{
    cameraActual->pan(dx,dy,0);
    cameraActual->update(program);
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    printf("press");
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    //si apretem el boto esquerre es canvien angles de visio de la camera
    if(event->buttons()&Qt::LeftButton)
    {
        if(lastPos.y()!=event->y()&&lastPos.x()!=event->x()){//es mou en diagonal(eix x i y)
            //moure la camera en angle x i/o en angle y

        } else if(lastPos.y()>event->y()){//es mou en eix y
            //moure camera angle x i/o angle y
            setYRotation(5);
        } else if(lastPos.y()<event->y()){//es mou en eix y
            //moure camera angle x i/o angle y
            setYRotation(-5);
        } else if(lastPos.x()>event->x()){//es mou en l'eix de les x
            setXRotation(5);
        } else if(lastPos.x()<event->x()){//es mou en l'eix de les x
            setXRotation(-5);
        }
    }else if(event->buttons()&Qt::RightButton){
        //panning: moure el centre de la window
        Pan(dx*2,dy*2);
    }else if(event->buttons()&Qt::MidButton){
        //Zoom canviar mida window un tant per cent
        if(lastPos.y()>event->y())//click boto del mig avall(reduir)
            Zoom(-1);
        else
            Zoom(1);
    }
    lastPos=event->pos();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    // Metode a implementar
    // P2: s'ha de moure la càmera en 3era persona junt amb el cotxe
    switch ( event->key() )
    {
    case Qt::Key_Up:
        if (esc->cotxe != NULL){
            if (esc->canMove(esc->cotxe,esc->cotxe->rodes.at(3)->yRot,0.05 ))
                esc->cotxe->forward();
            if (esc->cam_thirdperson == cameraActual)
                esc->actualitzaCameraThirdPerson();
        }

        break;
    case Qt::Key_Down:
        if (esc->cotxe != NULL){
            if (esc->canMove(esc->cotxe,esc->cotxe->rodes.at(3)->yRot,0.05 ))
                esc->cotxe->backward();
            if (esc->cam_thirdperson == cameraActual)
                esc->actualitzaCameraThirdPerson();
        }

        break;
    case Qt::Key_Left:
        if (esc->cotxe != NULL) esc->cotxe->turnleft();
        break;
    case Qt::Key_Right:
        if (esc->cotxe != NULL) esc->cotxe->turnright();

        break;
    case Qt::Key_Escape:
        if (cameraActual == esc->cam_thirdperson){
            cameraActual = esc->cam_panoramica;
            esc->resetCameraPanoramica();
        }
        else{
            cameraActual = esc->cam_thirdperson;
            esc->iniLookAtCotxe();
        }

        break;
    }

    cameraActual->update(program);
    updateGL();

}
void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    // Metode a implementar en el cas que es doni velocitat al cotxe

}



