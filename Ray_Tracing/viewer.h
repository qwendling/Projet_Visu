#ifndef VIEWER_H
#define VIEWER_H

#include <GL/glew.h>
#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedFrame.h>
#include <OGLRender/shaderprogramcolor.h>

#include <matrices.h>
#include <primitives.h>
#include <meshtri.h>
#include "AssetLoader.h"
#include <iostream>

#include "list_triangle.h"
#include "rayon.h"
#include "triangle.h"
#include "grid.h"
#include "ray_phong.h"
#include "ray_stochastique.h"
#include "source_facette.h"
#include "ray_photonmapping.h"

/**
 * @brief The Viewer class
 *
 *
 */
class Viewer : public QGLViewer
{
	Q_OBJECT

public:
    Viewer();
    void loadMesh(const std::string filename);
    int nb_mesh;
    std::vector<MeshTri> t_mesh;
    MeshTri debug;
    void mousePressEvent(QMouseEvent* event);

    //Line de Debug
    qglviewer::Vec orig;
    qglviewer::Vec dir;
    qglviewer::Vec orig_light;
    qglviewer::Vec dir_light;

    QColor bck;

    //Point Intersection Debug
    Triangle tri_inter;
    qglviewer::Vec pts_inter;
    bool hasIntersection = false;

    bool isRendu;

protected:

    void testRendu(int x, int y);
    void drawOverpaint(QPainter *painter);

    void paintEvent(QPaintEvent *event);
	/// OpenGL intialisation appelee par la QGLViewer
    void init();

	/// draw callback de la QGLViewer
    void draw();

	void animate();

	/// callback when key pressed
    void keyPressEvent(QKeyEvent *e);

    /// draw une ligne de debug
    void draw_debug_line();
    virtual void postSelection(const QPoint &point);

    /// draw interesction point if exist
    void draw_debug_inter_pts();

    /// Lancer de rayon (photon mapping)
    void rayTracing();

    /// Lancer de rayon (phong)
    void rayTracing_phong();

    /// Lancer de rayon (stochastique)
    void rayTracing_stochastique();

	/// recupere la matrice de modelview de la QGLViewer
	Mat4 getCurrentModelViewMatrix() const;

	/// recupere la matrice de modelview de la QGLViewer
	Mat4 getCurrentProjectionMatrix() const;

    Ray_phong* rp;
    Ray_stochastique* rs;
    std::vector<std::vector<Vec3>> Image;

    /// 0:flat 1:phong
	int m_render_mode;

    /// raccourcis couleurs
	Vec3 ROUGE;
	Vec3 VERT;
	Vec3 BLEU;
	Vec3 JAUNE;
	Vec3 CYAN;
	Vec3 MAGENTA;
	Vec3 BLANC;
	Vec3 GRIS;
	Vec3 NOIR;
    Vec3 lumiere;

    /// compteur animation
	int m_compteur;

    /// editeur de polygon
    AssetLoader* _Loader;
    Grid* grid_;
    std::vector<Cell> cell_passed;
    MeshTri facette_lumiere;
    std::vector<source_facette> liste_facettes;
    int id_active_facette;
    PhotonMap pm;
private slots:
    void initPainter();

};

#endif
