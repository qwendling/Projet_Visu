#-------------------------------------------------
#
# Project created by QtCreator 2018-10-04T22:22:46
#
#-------------------------------------------------


QT += core gui opengl xml widgets
TARGET = ray_trace
TEMPLATE = app

# include path for QGLViewer & glm
INCLUDEPATH += ..
INCLUDEPATH += $$_PRO_FILE_PWD_/../assimp/include

LIBS += $$_PRO_FILE_PWD_/../assimp/lib/libassimp.so

DESTDIR =$$_PRO_FILE_PWD_/../bin/

# define path for shaders
QMAKE_CXXFLAGS += -DSHADERPATH=$$_PRO_FILE_PWD_

# System dependent options

# Linux & macOS/X
unix {
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS +=  -Wl,-rpath,$$_PRO_FILE_PWD_/../bin
LIBS += -L$$_PRO_FILE_PWD_/../bin -lOGLRender -lQGLViewer33 -lglut
}

# Windows (64b)
win32 {
QMAKE_CXXFLAGS += -D_USE_MATH_DEFINES
QMAKE_CXXFLAGS_WARN_ON += -wd4267 -wd4244 -wd4305
LIBS += -L$$_PRO_FILE_PWD_/../bin -lOGLRender -lQGLViewer33 -lopengl32
}


SOURCES += main.cpp \
    viewer.cpp \
    meshtri.cpp \
    AssetLoader.cpp \
    rayon.cpp \
    triangle.cpp \
    source_facette.cpp \
    grid.cpp \
    cell.cpp \
    ray_phong.cpp \
    ray_stochastique.cpp

HEADERS  += viewer.h \
    matrices.h \
    meshtri.h \
    AssetLoader.h \
    rayon.h \
    triangle.h \
    source_facette.h \
    list_triangle.h \
    grid.h \
    cell.h \
    ray_phong.h \
    ray_stochastique.h


