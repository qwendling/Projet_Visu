#include "viewer.h"

#include <QKeyEvent>
#include <iomanip>
#include <GL/glut.h>
#include <algorithm>
#include <ray_photonmapping.h>
#define RESOLUTION_GRID 20

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
    isRendu(false),
    lumiere(-0.702039,3.9392,1.51081),
    id_active_facette(0)
{
    _Loader = new AssetLoader();

}


void Viewer::loadMesh(const std::string filename){
    bool statusOk = _Loader->import(filename);
    assert(statusOk);
    bool loadOk = _Loader->loadData(t_mesh);

    std::cout << "Load data : OK  " << std::endl;

    assert(loadOk);
    nb_mesh = _Loader->_scene->mNumMeshes;



    List_triangle lt;

    for(int i=0;i<nb_mesh;i++){
        for(auto& v:t_mesh[i].get_list_triangle()){
            lt.push_back(v);
        }
    }
    this->grid_ = new Grid(lt,RESOLUTION_GRID);

    std::cout << "Viewer load mesh : OK " << std::endl;
}

void Viewer::init()
{

    Triangle t(Vec3(0,0,0),Vec3(1,0,0),Vec3(0,0,1));
    source_facette sf(t);
    this->liste_facettes.push_back(sf);

    t = Triangle(Vec3(1,0,1),Vec3(0,0,1),Vec3(1,0,0));
    sf = source_facette(t);
    this->liste_facettes.push_back(sf);

    setMouseBinding(Qt::ShiftModifier, Qt::RightButton, QGLViewer::FRAME,
                    QGLViewer::TRANSLATE);
    setManipulatedFrame(new qglviewer::ManipulatedFrame());


	makeCurrent();

	glewExperimental = GL_TRUE;
	int glewErr = glewInit();
	if( glewErr != GLEW_OK )
	{
		qDebug("Error %s", glewGetErrorString(glewErr) ) ;
	}
	std::cout << "GL VERSION = " << glGetString(GL_VERSION) << std::endl;

    bck=QColor(40,40,0,255);

    glClearColor(bck.red()/255,bck.green()/255,bck.blue()/255,0.0);

	// QGLViewer initialisation de la scene
    setSceneRadius(20.0);
	setSceneCenter(qglviewer::Vec(0.0,0.0,0.0));
	camera()->showEntireScene();

	// initialisation variables globales
	m_compteur = 0;


    for(unsigned i=0;i<t_mesh.size();i++){
        t_mesh[i].gl_init();
    }
    debug.gl_init();
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

void Viewer::initPainter(){
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawOverpaint(&painter);

    painter.end();
}

void clamp(double& v,double low,double hi){
    assert(low<=hi);
    if((int)v<low)
        v = low;
    if((int)v>hi)
        v = hi;
    v=(int) v;
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
          double tmp_x,tmp_y,tmp_z;
          tmp_x = Image[i+W/2][j+H/2].x;
          clamp(tmp_x,0,255);

          tmp_y = Image[i+W/2][j+H/2].y;
          clamp(tmp_y,0,255);

          tmp_z = Image[i+W/2][j+H/2].z;
          clamp(tmp_z,0,255);

          pen.setColor(QColor(tmp_x,tmp_y,tmp_z));
          painter->setPen(pen);
          painter->drawPoint(i,j);

      }
  }
  painter->restore();
}


void Viewer::draw()
{

	makeCurrent();
    Vec3 pos_frame(manipulatedFrame()->position().x,manipulatedFrame()->position().y,manipulatedFrame()->position().z);
    liste_facettes[id_active_facette].move_facette(pos_frame);

    for(unsigned i=0;i<t_mesh.size();i++){
        t_mesh[i].set_matrices(getCurrentModelViewMatrix(),getCurrentProjectionMatrix());
        t_mesh[i].gl_update();
    }
    debug.set_matrices(getCurrentModelViewMatrix(),getCurrentProjectionMatrix());
    debug.gl_update();

    if (m_render_mode==0){
        for(unsigned i=0;i<t_mesh.size();i++){
            t_mesh[i].draw(Vec3(t_mesh[i].getColor().r,t_mesh[i].getColor().g,t_mesh[i].getColor().b));
        }
        debug.draw(VERT);
    }

    if (m_render_mode==1){
        for(unsigned i=0;i<t_mesh.size();i++){
            t_mesh[i].draw_smooth(Vec3(t_mesh[i].getColor().r,t_mesh[i].getColor().g,t_mesh[i].getColor().b));
        }
        debug.draw_smooth(VERT);
    }

   //Dessine la Ligne de Debug
   draw_debug_line();

   if(hasIntersection){
       draw_debug_inter_pts();
   }
   for(auto& c:cell_passed)
       c.draw();

   for(unsigned i=0;i<liste_facettes.size();i++){
       if(i!=id_active_facette){
           liste_facettes[i].draw(1,1,1);
       }else{
           liste_facettes[i].draw(0,1,0);
       }
   }

   glColor4f(1,1,1,1) ;
   glBegin(GL_POINTS) ;
   for(auto& p:pm){
       glVertex3f(p->position.x,p->position.y,p->position.z) ;
   }
   glEnd() ;
}

void Viewer::rayTracing(){
    Image.resize(this->width());
    for(auto& i:Image){
        i.resize(this->height());
        for(auto& v:i){
            v = Vec3(1,0,1);
        }
    }

    ray_photonmapping* rpm = new ray_photonmapping(Image,*camera(),*grid_,bck);
    rpm->add_lumiere(this->lumiere);
    for(auto& sf:liste_facettes)
        rpm->add_facette(sf);
    connect(rpm,SIGNAL(update_draw()),SLOT(initPainter()));
    rpm->compute_indirect();
    initPainter();
    std::cout << "fin ray trace" << std::endl;

}

void Viewer::rayTracing_phong(){
    Image.resize(this->width());
    for(auto& i:Image){
        i.resize(this->height());
        for(auto& v:i){
            v = Vec3(1,0,1);
        }
    }

    rp = new Ray_phong(Image,*camera(),*grid_,bck);
    rp->add_lumiere(this->lumiere);
    connect(rp,SIGNAL(update_draw()),SLOT(initPainter()));
    rp->compute_phong();
    initPainter();
}

void Viewer::rayTracing_stochastique(){
    Image.resize(this->width());
    for(auto& i:Image){
        i.resize(this->height());
        for(auto& v:i){
            v = Vec3(1,0,1);
        }
    }


    rs = new Ray_stochastique(Image,*camera(),*grid_,bck);
    rs->add_lumiere(this->lumiere);
    for(auto& sf:liste_facettes)
        rs->add_facette(sf);
    connect(rs,SIGNAL(update_draw()),SLOT(initPainter()));
    rs->compute();
    initPainter();

    std::cout << "fin ray trace" << std::endl;

}

void Viewer::keyPressEvent(QKeyEvent *e)
{
    Vec3 cm ;
    ray_photonmapping* rpm;
	switch(e->key())
	{
		case Qt::Key_Escape:
			exit(EXIT_SUCCESS);
			break;

		case Qt::Key_M: // touche 'x'
				m_render_mode = (m_render_mode+1)%2;
		break;

        case Qt::Key_Enter:
               isRendu = true;
               rayTracing();
        break;
        case Qt::Key_Tab:
            id_active_facette = (id_active_facette+1)%liste_facettes.size();
            cm = liste_facettes[id_active_facette].get_cm();
            manipulatedFrame()->setPosition(cm.x,cm.y,cm.z);
        break;

    case  Qt::Key_P:
        lumiere = Vec3(camera()->position().x,camera()->position().y,camera()->position().z);
        std::cout << "pos camera : " << camera()->position() << std::endl;
        break;
    case  Qt::Key_R:
        isRendu = true;
        rayTracing_phong();
        break;
    case  Qt::Key_B:
        isRendu = true;
        rayTracing_stochastique();
        break;
    case  Qt::Key_S:
        isRendu = false;
        break;
    case Qt::Key_T:
        rpm = new ray_photonmapping(Image,*camera(),*grid_,bck);
        for(auto& sf:liste_facettes)
            rpm->add_facette(sf);
        pm = rpm->compute_photonMap();
        std::cout << "photon map compute " << pm.size() << std::endl;

        break;
		default:
			break;
	}

	// retrace la fenetre
    if(!isRendu)
        update();

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

void Viewer::postSelection(const QPoint &point)
{
    camera()->convertClickToLine(point, orig, dir);

    std::cout << "(" << point.x() << "," << point.y() << ")" << std::endl;

    Vec3 o (orig[0],orig[1],orig[2]);
    Vec3 d (dir[0],dir[1],dir[2]);

    Rayon r (o,d);
    Vec3 pts;



    for(int i=0; i < t_mesh.size();i++){
        List_triangle list =  t_mesh[i].get_list_triangle();
        if(r.intersecListeTri(list,tri_inter,pts)){
            pts = Vec3();
            cell_passed.clear();
            this->grid_->intersec_ray(r,tri_inter,pts,cell_passed,-1);
            hasIntersection = true;
            Rayon tmp(lumiere,pts-lumiere);

            orig_light = qglviewer::Vec(lumiere.x,lumiere.y,lumiere.z);
            dir_light = qglviewer::Vec(tmp.get_direction().x,tmp.get_direction().y,tmp.get_direction().z);
            pts = Vec3(0,0,0);
            if(this->grid_->intersec_ray(tmp,tri_inter,pts,tri_inter.index))
                std::cout << "ombre" << std::endl;
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

        }
        break;
    }

    QGLViewer::mousePressEvent(event);
    update();

}


