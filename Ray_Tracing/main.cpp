#include <QApplication>
#include "viewer.h"
#include "view2d.h"
#include "AssetLoader.h"
#include "rayon.h"
#include "source_facette.h"
#include "GL/glut.h"
#include <GL/glew.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    srand (time(NULL));

    /*glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
      glutInitWindowPosition(0,0) ;
      glutInitWindowSize(500, 500);
      int win = glutCreateWindow("Rendu");
      GLenum err = glewInit();
      if (GLEW_OK != err)
        {
        /* Problem: glewInit failed, something is seriously wrong. */
        /*fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        }
    */

    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    //MDR
    //glFormat.setSampleBuffers( true );
    //QGLFormat::setDefaultFormat(glFormat);

    Viewer view;
    view.loadMesh("../Model3D/cube.obj");
    view.setGeometry(10,20,500,500);
    view.show();

    return a.exec();
}
