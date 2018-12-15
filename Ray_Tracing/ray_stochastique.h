#ifndef RAY_STOCHASTIQUE_H
#define RAY_STOCHASTIQUE_H

#include "ray_phong.h"
#include "source_facette.h"


class Ray_stochastique : public QObject
{
    Q_OBJECT
public:
    Ray_stochastique()=delete;
    Ray_stochastique(std::vector<std::vector<Vec3>>& im,qglviewer::Camera& cam,Grid& gr,QColor& bck):Image(im),camera(cam),grille(gr),Background_color(bck){}
    void compute();
    void compute_aux(int debut,int fin);
    std::vector<source_facette> liste_facettes;
    void add_facette(source_facette& sf){liste_facettes.push_back(sf);}
    void add_lumiere(Vec3 l){liste_lumiere.push_back(l);}
    std::vector<std::vector<Vec3>>& Image;
    qglviewer::Camera& camera;
    Grid& grille;
    QColor Background_color;
    std::vector<Vec3> liste_lumiere;
    QTimer clock_update;
signals:
    void update_draw();
protected slots:
    void slot_emit_update(){emit update_draw();std::cout << "ok" << std::endl;}

};

#endif // RAY_STOCHASTIQUE_H
