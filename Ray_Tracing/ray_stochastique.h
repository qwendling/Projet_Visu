#ifndef RAY_STOCHASTIQUE_H
#define RAY_STOCHASTIQUE_H

#include "ray_phong.h"


class Ray_stochastique : public Ray_phong
{
public:
    Ray_stochastique()=delete;
    Ray_stochastique(std::vector<std::vector<Vec3>>& im,qglviewer::Camera& cam,Grid& gr,QColor& bck):Ray_phong(im,cam,gr,bck){}
    void compute();
};

#endif // RAY_STOCHASTIQUE_H
