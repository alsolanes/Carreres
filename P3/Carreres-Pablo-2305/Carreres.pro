#-------------------------------------------------
#
# Project created by QtCreator 2013-03-05T16:12:33
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Carreres
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    objecte.cpp \
    Common.cpp \
    cara.cpp \
    escena.cpp \
    cotxe.cpp \
    newcotxedialog.cpp \
    newobstacledialog.cpp \
    newterradialog.cpp \
    terra.cpp \
    carroceria.cpp \
    roda.cpp \
    obstacle.cpp \
    readfile.cpp \
    trajectoria.cpp \
    camera.cpp \
    conjuntllums.cpp \
    llum.cpp \
    material.cpp \
    conjuntmaterials.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    objecte.h \
    Common.h \
    cara.h \
    vec.h \
    mat.h \
    escena.h \
    cotxe.h \
    newcotxedialog.h \
    newobstacledialog.h \
    newterradialog.h \
    readfile.h \
    terra.h \
    carroceria.h \
    roda.h \
    obstacle.h \
    trajectoria.h \
    camera.h \
    conjuntllums.h \
    llum.h \
    material.h \
    conjuntmaterials.h

FORMS    += mainwindow.ui \
    newcotxedialog.ui \
    newterradialog.ui \
    newobstacledialog.ui

OTHER_FILES += \
    vshader11.glsl \
    fshader11.glsl \
    vshader1.glsl \
    fshader1.glsl
