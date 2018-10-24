#include "viewer.h"

#include <QKeyEvent>
#include <iomanip>


Viewer::Viewer():
	QGLViewer(),
	m_render_mode(0),
	ROUGE(1,0,0),
	VERT(0,1,0),
	BLEU(0,0,1),
	JAUNE(1,1,0),
	CYAN(0,1,1),
	MAGENTA(1,0,1),
	BLANC(1,1,1),
	GRIS(0.5,0.5,0.5),
    NOIR(0,0,0)
{
    _Loader = new AssetLoader();
}


void Viewer::loadMesh(const std::string filename){
    bool statusOk = _Loader->import(filename);
    assert(statusOk);
    bool loadOk = _Loader->loadData(t_mesh);

    assert(loadOk);
    nb_mesh = _Loader->_scene->mNumMeshes;
}

void Viewer::init()
{
	makeCurrent();
	glewExperimental = GL_TRUE;
	int glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		qDebug("Error %s", glewGetErrorString(glewErr) ) ;
	}
	std::cout << "GL VERSION = " << glGetString(GL_VERSION) << std::endl;

	// la couleur de fond
	glClearColor(0.2,0.2,0.2,0.0);

	// QGLViewer initialisation de la scene
	setSceneRadius(2.0);
	setSceneCenter(qglviewer::Vec(0.0,0.0,0.0));
	camera()->showEntireScene();

	// initialisation variables globales
	m_compteur = 0;

	m_mesh.gl_init();
    for(unsigned i=0;i<t_mesh.size();i++){
        t_mesh[i].gl_init();
    }
}




void Viewer::draw()
{
	makeCurrent();

    for(unsigned i=0;i<t_mesh.size();i++){
        t_mesh[i].set_matrices(getCurrentModelViewMatrix(),getCurrentProjectionMatrix());
        t_mesh[i].gl_update();
    }

	m_mesh.set_matrices(getCurrentModelViewMatrix(),getCurrentProjectionMatrix());

    if (m_render_mode==0){
        for(unsigned i=0;i<t_mesh.size();i++){
            t_mesh[i].draw(ROUGE);
        }
		m_mesh.draw(ROUGE);
    }

    if (m_render_mode==1){
        for(unsigned i=0;i<t_mesh.size();i++){
            t_mesh[i].draw_smooth(ROUGE);
        }
        m_mesh.draw_smooth(ROUGE);
    }

	if (m_render_mode==1)
        m_mesh.draw_smooth(ROUGE);
}


void Viewer::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
		case Qt::Key_Escape:
			exit(EXIT_SUCCESS);
			break;

		case Qt::Key_F: // touche 'w'
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			break;
		case Qt::Key_G: // touche 'w'
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;

		case Qt::Key_P:
			m_mesh.create_pyramide();
		break;

		case Qt::Key_A:
			m_mesh.create_anneau();
		break;

		case Qt::Key_S:
			m_mesh.create_spirale();
		break;

		break;

		case Qt::Key_N: // touche 'x'
				m_mesh.compute_normals();
		break;

		case Qt::Key_M: // touche 'x'
				m_render_mode = (m_render_mode+1)%2;
		break;
		default:
			break;
	}

	// retrace la fenetre
	updateGL();
    //QGLViewer::keyPressEvent(e);
}



void Viewer::animate()
{
	m_compteur += 1;
}



Mat4 Viewer::getCurrentModelViewMatrix() const
{
	GLdouble gl_mvm[16];
	camera()->getModelViewMatrix(gl_mvm);
	Mat4 mvm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			mvm[i][j] = float(gl_mvm[i*4+j]);
	}
	return mvm;
}


Mat4 Viewer::getCurrentProjectionMatrix() const
{
	GLdouble gl_pm[16];
	camera()->getProjectionMatrix(gl_pm);
	Mat4 pm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			pm[i][j] = float(gl_pm[i*4+j]);
	}
	return pm;
}


void Viewer::rayClick(QMouseEvent* event, qglviewer::Vec& P, qglviewer::Vec& Q)
{
    P = camera()->unprojectedCoordinatesOf(qglviewer::Vec(event->x(), event->y(), -1.0));
    Q = camera()->unprojectedCoordinatesOf(qglviewer::Vec(event->x(), event->y(), 1.0));
}

void Viewer::mousePressEvent(QMouseEvent* event)
{
    qglviewer::Vec P;
    qglviewer::Vec Q;
    rayClick(event, P, Q);
    Vec3 A(P[0], P[1], P[2]);
    Vec3 B(Q[0], Q[1], Q[2]);


    if ((event->modifiers() & Qt::ShiftModifier))
    {

    }

    QGLViewer::mousePressEvent(event);
    update();
}
