#include "ray_photonmapping.h"

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
