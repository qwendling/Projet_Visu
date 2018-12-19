#include "ray_photonmapping.h"
#include <cstdlib>
#include <ctime>
#include <random>

void ray_photonmapping::compute(){
    return;
}

PhotonMap ray_photonmapping::compute_photonMap(){
    PhotonMap result;

    for(auto& t:liste_facettes){
        for(auto& l:t.liste_sources){
            //lancer N rayon dans une sphere -> operateur rayon dans sphere
            //foreach rayon :
            //  continue = true;
            //  while(continue)
            //  compute intersect
            //  if intersect :
            //      create photon
            //      continue = roulette()
            //  else
            //      continue = false

        }
    }

    return result;
}


Vec3 ray_photonmapping::get_random_dir_in_sphere(){
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f,1.0f);

    double alpha = 2*M_PI*distribution(generator);
    double betha = acos(1-2*distribution(generator));

    Vec3 result(cos(alpha)*sin(betha),sin(alpha)*sin(betha),cos(betha));

    return result;
}
