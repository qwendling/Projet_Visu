#ifndef RAY_PHOTONMAPPING_H
#define RAY_PHOTONMAPPING_H

#include "ray_stochastique.h"
#include "photon.h"

class ray_photonmapping : public Ray_stochastique
{
public:

    void compute();

    PhotonMap compute_photonMap();
    void compute_indirect();

    ray_photonmapping() =delete;
    ray_photonmapping(std::vector<std::vector<Vec3>>& im,qglviewer::Camera& cam,Grid& gr,QColor& bck):Ray_stochastique(im,cam,gr,bck){}

    std::vector<Vec3> get_random_dir_in_sphere();
};

#endif // RAY_PHOTONMAPPING_H
