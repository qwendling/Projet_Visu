#include "viewer.h"

#include <QKeyEvent>
#include <iomanip>
#include <GL/glut.h>
#define RESOLUTION_GRID 10

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
    NOIR(0,0,0),
    isRendu(false)
{
    _Loader = new AssetLoader();
}


void Viewer::loadMesh(const std::string filename){
    bool statusOk = _Loader->import(filename);
    assert(statusOk);
    bool loadOk = _Loader->loadData(t_mesh);

    assert(loadOk);
    nb_mesh = _Loader->_scene->mNumMeshes;

    List_triangle lt;

    for(int i=0;nb_mesh<nb_mesh;i++){
        lt.insert(lt.end(),t_mesh[i].get_list_triangle().begin(),t_mesh[i].get_list_triangle().end());
    }
    this->grid_ = new Grid(lt,RESOLUTION_GRID);
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


    for(unsigned i=0;i<t_mesh.size();i++){
        t_mesh[i].gl_init();
    }
}



void Viewer::paintEvent(QPaintEvent *event) {

  Q_UNUSED(event)
  QPainter painter;
  painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Save current OpenGL state
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Reset OpenGL parameters
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_MULTISAMPLE);
  static GLfloat lightPosition[4] = {1.0, 5.0, 5.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glClearColor(backgroundColor().redF(), backgroundColor().greenF(),
               backgroundColor().blueF(), backgroundColor().alphaF());

  // Classical 3D drawing, usually performed by paintGL().
  preDraw();
  draw();
  postDraw();
  // Restore OpenGL state
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glPopAttrib();

  drawOverpaint(&painter);

  painter.end();
}

void Viewer::drawOverpaint(QPainter *painter) {

    if(!isRendu){
        return;
    }

  painter->save();
  painter->translate(width() / 2, height() / 2);
  QPen pen;

  int W = width();
  int H = height();
  for (int j=-H/2; j<H/2; j++)
  {
      for (int i=-W/2; i<W/2; i++)
      {
        if(i%20 == 0){
            pen.setColor(QColor(0,255,0,255));
            painter->setPen(pen);
            painter->drawPoint(i,j);
        }else{
            pen.setColor(QColor(255,0,255,255));
            painter->setPen(pen);
            painter->drawPoint(i,j);
        }
      }
  }

  painter->restore();
}


void Viewer::draw()
{
	makeCurrent();

    for(unsigned i=0;i<t_mesh.size();i++){
        t_mesh[i].set_matrices(getCurrentModelViewMatrix(),getCurrentProjectionMatrix());
        t_mesh[i].gl_update();
    }

    if (m_render_mode==0){
        for(unsigned i=0;i<t_mesh.size();i++){
            t_mesh[i].draw(Vec3(t_mesh[i].getColor().r,t_mesh[i].getColor().g,t_mesh[i].getColor().b));
        }
    }

    if (m_render_mode==1){
        for(unsigned i=0;i<t_mesh.size();i++){
            t_mesh[i].draw_smooth(Vec3(t_mesh[i].getColor().r,t_mesh[i].getColor().g,t_mesh[i].getColor().b));
        }
    }

   //Dessine la Ligne de Debug
   draw_debug_line();

   if(hasIntersection){
       draw_debug_inter_pts();
   }

}

void Viewer::rayTracing(){

}


void Viewer::keyPressEvent(QKeyEvent *e)
{
	switch(e->key())
	{
		case Qt::Key_Escape:
			exit(EXIT_SUCCESS);
			break;

		case Qt::Key_M: // touche 'x'
				m_render_mode = (m_render_mode+1)%2;
		break;

        case Qt::Key_Enter:
               isRendu = !isRendu;
        break;
		default:
			break;
	}

	// retrace la fenetre

    update();


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


void Viewer::draw_debug_line(){

    glLineWidth(2.0);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3fv(orig);
    glVertex3fv(orig + 100.0 * dir);
    glEnd();

}


void Viewer::draw_debug_inter_pts(){
    glPointSize(10.0);
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    glVertex3fv(pts_inter);
    glEnd();

}

//Shift + clic action
void Viewer::postSelection(const QPoint &point)
{
    camera()->convertClickToLine(point, orig, dir);

    Vec3 o (orig[0],orig[1],orig[2]);
    Vec3 d (dir[0],dir[1],dir[2]);

    Rayon r (o,d);
    Vec3 pts;



    for(int i=0; i < t_mesh.size();i++){
        List_triangle list =  t_mesh[i].get_list_triangle();
        if(r.intersecListeTri(list,tri_inter,pts)){
            hasIntersection = true;
            break;
        }else {
            hasIntersection = false;
        }
    }

    pts_inter.setValue(pts[0], pts[1], pts[2]);

    update();
}

void Viewer::mousePressEvent(QMouseEvent* event)
{
    switch(event->button())
    {
        case Qt::LeftButton:
        {
            //testRendu(event->x(),event->y());
        }
        break;
    }

    QGLViewer::mousePressEvent(event);
    update();

}


