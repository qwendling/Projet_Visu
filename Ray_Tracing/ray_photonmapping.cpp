#include "ray_photonmapping.h"
#include <cstdlib>
#include <ctime>
#include <random>

#define NB_RAY_FROM_LIGHT 100

void ray_photonmapping::compute(){
    return;
}

PhotonMap ray_photonmapping::compute_photonMap(){
    PhotonMap result;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f,1.0f);
    for(auto& t:liste_facettes){
        for(auto& l:t.liste_sources){
            std::vector<Vec3> liste_dir = get_random_dir_in_sphere();
            for(auto& dir:liste_dir){
                Rayon rayon_photon(l,dir);
                Triangle tmp_tri;
                Vec3 tmp_inter;
                int id = -1;
                bool rebond = true;
                double energie = 1/(NB_RAY_FROM_LIGHT*t.liste_sources.size());

                while(rebond){
                    rebond = false;
                    if(grille.intersec_ray(rayon_photon,tmp_tri,tmp_inter,id)){
                        Vec3 tmp = rayon_photon.get_direction();
                        result.push_back(new Photon(tmp,tmp_inter,energie,tmp_tri));
                        float r = distribution(generator);
                        if( r > 0.5f){
                            energie /= 2.0f;
                            rebond = true;
                            id = tmp_tri.index;
                            rayon_photon = Rayon(glm::reflect(tmp,tmp_tri.computeNormal()),tmp_inter);
                        }
                    }
                }
            }
        }
    }

    return result;
}


std::vector<Vec3> ray_photonmapping::get_random_dir_in_sphere(){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f,1.0f);

    std::vector<Vec3> result;
    for(int i=0;i<NB_RAY_FROM_LIGHT;i++){
        double alpha = 2*M_PI*distribution(generator);
        double betha = acos(1-2*distribution(generator));

        Vec3 r(cos(alpha)*sin(betha),sin(alpha)*sin(betha),cos(betha));
        result.push_back(r);
    }

    return result;
}
