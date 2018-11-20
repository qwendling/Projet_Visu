#ifndef RAY_PHONG_H
#define RAY_PHONG_H

#include <QObject>
#include <vector>
#include <matrices.h>
#include "grid.h"
#include <GL/glew.h>
#include <QGLViewer/qglviewer.h>
#include <OGLRender/shaderprogramcolor.h>

typedef glm::vec3 Vec3;

class Ray_phong:public QObject
{
    Q_OBJECT
public:
    Ray_phong()=delete;
    Ray_phong(std::vector<std::vector<Vec3>>& im,qglviewer::Camera& cam,Grid& gr,QColor& bck):Image(im),camera(cam),grille(gr),Background_color(bck){}
    std::vector<std::vector<Vec3>>& Image;
    qglviewer::Camera& camera;
    Grid& grille;
    QColor Background_color;

    void compute_phong();
signals:
    void update_draw();
};

#endif // RAY_PHONG_H
