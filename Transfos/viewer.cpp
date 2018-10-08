#include "viewer.h"

#include <QKeyEvent>
#include <iomanip>


Viewer::Viewer():
	QGLViewer(),
	ROUGE(1,0,0),
	VERT(0,1,0),
	BLEU(0,0,1),
	JAUNE(1,1,0),
	CYAN(0,1,1),
	MAGENTA(1,0,1),
	BLANC(1,1,1),
	GRIS(0.5,0.5,0.5),
	NOIR(0,0,0),
    m_code(1)
{}


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
	setSceneRadius(9.0);
	setSceneCenter(qglviewer::Vec(0.0,0.0,0.0));
	camera()->showEntireScene();

	// initilisation des primitives
	m_prim.gl_init();

	// initialisation variables globales
	m_compteur = 0;
	m_angle1 = 0.0;
	m_angle2 = 0.0;


}



void Viewer::draw_repere(const Mat4& global)
{
	//	// exemple de definition de fonction (lambda) locale
	//	float b=2.2f;
	//	auto fonction_locale = [&] (float a)
	//	{
	//		std::cout << "param a="<< a << " & global b="<< b <<std::endl;
	//	};
	//	//appel
	//	fonction_locale(1.1f);
    m_prim.draw_sphere(global,BLANC);
    m_prim.draw_cylinder(global*translate(1.5,0,0)*rotateY(90)*scale(0.5,0.5,3),ROUGE);
    m_prim.draw_cylinder(global*translate(0,1.5,0)*rotateX(90)*scale(0.5,0.5,3),VERT);
    m_prim.draw_cylinder(global*translate(0,0,1.5)*scale(0.5,0.5,3),BLEU);
    m_prim.draw_cone(global*translate(3,0,0)*rotateY(90),ROUGE);
    m_prim.draw_cone(global*translate(0,3,0)*rotateX(-90),VERT);
    m_prim.draw_cone(global*translate(0,0,3)*rotateZ(90),BLEU);

}

double tmpferm=0;

void Viewer::draw_main()
{

    auto draw_doigt = [&] (Mat4 global,const Mat4& fermeture) -> void{
        m_prim.draw_sphere(global,BLANC);
        m_prim.draw_cube(global*fermeture*translate(1,0,0)*scale(2,0.8,0.5),ROUGE);
        global *=fermeture*translate(2,0,0);

        m_prim.draw_sphere(global,BLANC);
        m_prim.draw_cube(global*fermeture*translate(1,0,0)*scale(2,0.8,0.5),VERT);

        global *=fermeture*translate(2,0,0);
        m_prim.draw_sphere(global,BLANC);
        m_prim.draw_cube(global*fermeture*translate(0.9,0,0)*scale(1.8,0.8,0.5),BLEU);
    };

    //draw de l'epaule
    m_prim.draw_cube(scale(5,4,0.95),CYAN);
    m_prim.draw_sphere(translate(-3,0,0),BLANC);
    Mat4 pos_epaule=translate(6,0,0);

    //draw du poignet
    m_prim.draw_cube(pos_epaule*scale(5,2,0.95),CYAN);
    m_prim.draw_sphere(pos_epaule*translate(-3,0,0),BLANC);
    Mat4 pos_poignet = pos_epaule*translate(5,0,0);

    //draw de la main
    m_prim.draw_cube(pos_poignet*scale(3,4,0.95),CYAN);
    m_prim.draw_sphere(pos_poignet*translate(-2,0,0),BLANC);
    Mat4 pos_main=pos_poignet*translate(1.5,-0.5,0);

    //draw des 5 doigts
    draw_doigt(pos_main,rotateY(tmpferm));
    draw_doigt(pos_main*translate(0,1,0)*scale(0.9,1,1),rotateY(tmpferm));
    draw_doigt(pos_main*translate(0,-1,0)*scale(0.9,1,1),rotateY(tmpferm));
    draw_doigt(pos_main*translate(0,2,0)*scale(0.75,1,1),rotateY(tmpferm));
    draw_doigt(pos_main*translate(-1.3,-1.75,0)*rotateZ(-50)*scale(0.6,1,1),rotateY(tmpferm));
}

void Viewer::draw_basic()
{
	m_prim.draw_sphere(Mat4(), BLANC);
	m_prim.draw_cube(translate(3,0,0), ROUGE);
	m_prim.draw_cone(translate(0,3,0), VERT);
	m_prim.draw_cylinder(translate(0,0,3), BLEU);
    m_prim.draw_sphere(translate(4,0,0), ROUGE);
}

float temp=0;
float temp2=cos(temp);
float angleglob=0;
void Viewer::draw()
{
	makeCurrent();
	m_prim.set_matrices(getCurrentModelViewMatrix(),getCurrentProjectionMatrix());

	Mat4 glob;

	switch(m_code)
	{
		case 0:
			draw_basic();
		break;
		case 1:
			draw_repere(glob);
		break;
		case 2:
			// coder ici les petits reperes qui tournet autour du grand repere
            draw_repere(glob);
            glob=glob*translate(10+temp2,0,0)*rotateY(90)*rotateX(angleglob);
            for(int i=0;i<12;i++){
                glob=rotateY(30)*glob;
                draw_repere(glob);
            }

		break;
		case 3:
			draw_main();
		break;
	}
}


void Viewer::keyPressEvent(QKeyEvent *e)
{

	if (e->modifiers() & Qt::ShiftModifier)
	{
		std::cout << "Shift is on"<< std::endl;
	}

	if (e->modifiers() & Qt::ControlModifier)
	{
		std::cout << "Control is on"<< std::endl;
	}

	switch(e->key())
	{
		case Qt::Key_Escape:
			exit(EXIT_SUCCESS);
			break;

		case Qt::Key_A: // touche 'a'
			if (animationIsStarted())
				stopAnimation();
			else
				startAnimation();
			break;

		case Qt::Key_M:  // change le code execute dans draw()
			m_code = (m_code+1)%4;
			break;
		default:
			break;
	}

	// retrace la fenetre
	updateGL();
	// passe la main a la QGLViewer
	QGLViewer::keyPressEvent(e);
}



void Viewer::animate()
{
	m_compteur += 1;
    temp+=0.1;
    angleglob+=10;
    temp2=cos(temp)*10;
    tmpferm=cos(temp)*45+45;

	// faire varier les angles ici pour animer
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
