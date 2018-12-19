#ifndef RAY_PHOTONMAPPING_H
#define RAY_PHOTONMAPPING_H

#include "ray_stochastique.h"
#include "photon.h"

class ray_photonmapping : public Ray_stochastique
{
public:

    void compute();

    PhotonMap compute_photonMap();

    ray_photonmapping();

    Vec3 get_random_dir_in_sphere();
};

#endif // RAY_PHOTONMAPPING_H
