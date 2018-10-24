#include <QApplication>
#include "viewer.h"
#include "AssetLoader.h"
#include "rayon.h"
#include "source_facette.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    srand (time(NULL));

    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );
	QGLFormat::setDefaultFormat(glFormat);

    Viewer view;
    view.loadMesh("../Model3D/cube.obj");
	view.setGeometry(10,20,500,500);
    view.show();

	return a.exec();

}
