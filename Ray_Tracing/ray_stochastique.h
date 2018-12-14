#ifndef RAY_STOCHASTIQUE_H
#define RAY_STOCHASTIQUE_H

#include "ray_phong.h"
#include "source_facette.h"


class Ray_stochastique : public Ray_phong
{
public:
    Ray_stochastique()=delete;
    Ray_stochastique(std::vector<std::vector<Vec3>>& im,qglviewer::Camera& cam,Grid& gr,QColor& bck):Ray_phong(im,cam,gr,bck){}
    void compute();
    void compute_aux(int debut,int fin);
    std::vector<source_facette> liste_facettes;
    void add_facette(source_facette& sf){liste_facettes.push_back(sf);}
};

#endif // RAY_STOCHASTIQUE_H
