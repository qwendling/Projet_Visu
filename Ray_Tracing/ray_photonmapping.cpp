#include "ray_photonmapping.h"


#define NB_RAY_FROM_LIGHT 500
#define USE_FINAL_GATHERING 1
#define USE_DIRECT 1

void ray_photonmapping::compute(){
    compute_photonMap();
}

inline double gaussian(const Vec3& x,double sigma){
    double n = 1.0f/(sqrt(2*M_PI)*sigma);
    return n * std::exp(-(x.x*x.x+x.y*x.y+x.z*x.z)/(2.0f*sigma*sigma));
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
                double energie = 100.0f/(double)(NB_RAY_FROM_LIGHT);

                while(rebond){
                    rebond = false;
                    if(grille.intersec_ray(rayon_photon,tmp_tri,tmp_inter,id)){
                        Vec3 tmp = rayon_photon.get_direction();
                        result.push_back(new Photon(tmp,tmp_inter,energie,tmp_tri));
                        float r = distribution(generator);
                        if( r > 0.2f){
                            rebond = true;
                            id = tmp_tri.index;
                            Vec3 R = glm::reflect(tmp,tmp_tri.computeNormal());
                            rayon_photon = Rayon(tmp_inter,R);
                        }
                    }
                }
            }
        }
    }


    pcl::PointCloud<pcl::PointXYZ> photon_cloud;

    for(Photon* p : result){
        pcl::PointXYZ tmp_p(p->position.x,p->position.y,p->position.z);
        photon_cloud.push_back(tmp_p);
    }

    result.kdtree.setInputCloud(photon_cloud.makeShared());

    return result;
}

void ray_photonmapping::compute_indirect(){
    PhotonMap pm = this->compute_photonMap();

    bool need_thread = true;
    int debut = 0;
    int fin;
    std::vector<QFuture<void>> liste_threads;

    std::vector<Vec3> liste_pixel_random = random_pixels(0,0);
    auto aux = [&](int debut,int fin){
        qglviewer::Vec orig;
        qglviewer::Vec dir;
        for(unsigned i=debut;i<fin;i++){
            for(unsigned j=0;j<Image[i].size();j++){
                Image[i][j] = Vec3(0,0,0);

                for(unsigned n = 0;n<liste_pixel_random.size();n++){
                        orig = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x+i,liste_pixel_random[n].y+j,0));
                        dir = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x+i,liste_pixel_random[n].y+j,1));

                        Vec3 o (orig[0],orig[1],orig[2]);
                        Vec3 d (dir[0],dir[1],dir[2]);
                        d=d-o;

                        Rayon r (o,d);

                        Triangle t_inter;
                        Vec3 inter;
                        if(grille.intersec_ray(r,t_inter,inter)){

#if USE_DIRECT
                            for(auto& t:liste_facettes){
                                for(auto& l:t.liste_sources){
                                    Rayon r_light(l,inter-l);
                                    Triangle tmp_tri;
                                    Vec3 tmp_inter;
                                    if(!grille.intersec_ray(r_light,tmp_tri,tmp_inter,t_inter.index)){
                                        Vec3 Kd_normalize = Vec3(255*t_inter.Kd.r,255*t_inter.Kd.g,255*t_inter.Kd.b)/(float)M_PI;

                                        Vec3 N = t_inter.computeNormal();
                                        Vec3 L = glm::normalize(l-inter);
                                        float cos_theta = glm::dot(L,N);

                                        //Bon ca marche mais pas ouf #scotch
                                        if(t_inter.Ns < 1)
                                            t_inter.Ns = 1;

                                        L= get_random_dir_in_cone(L,M_PI/(2.0f*(float)t_inter.Ns));
                                        Vec3 R = glm::reflect(-L,N);


                                        //Vec3 R = glm::normalize(2*cos_theta*N-L);

                                        Vec3 color_diff = Kd_normalize*cos_theta;



                                        Vec3 Ks_normalise = Vec3(255*t_inter.Ks.r,255*t_inter.Ks.g,255*t_inter.Ks.b)/(float)(t_inter.Ns+2);
                                        Vec3 color_spec = Ks_normalise*(float)pow(glm::dot(R,glm::normalize(-r.get_direction())),t_inter.Ns);

                                        if(color_spec.x < 0)
                                            color_spec.x=0;
                                        if(color_spec.y < 0)
                                            color_spec.y=0;
                                        if(color_spec.z < 0)
                                            color_spec.z=0;

                                        if(color_spec.x > 255)
                                            color_spec.x=255;
                                        if(color_spec.y > 255)
                                            color_spec.y=255;
                                        if(color_spec.z > 255)
                                            color_spec.z=255;

                                        if(color_diff.x < 0)
                                            color_diff.x=0;
                                        if(color_diff.y < 0)
                                            color_diff.y=0;
                                        if(color_diff.z < 0)
                                            color_diff.z=0;

                                        if(color_diff.x > 255)
                                            color_diff.x=255;
                                        if(color_diff.y > 255)
                                            color_diff.y=255;
                                        if(color_diff.z > 255)
                                            color_diff.z=255;

                                        Image[i][j] += (color_diff + color_spec)/(float)t.liste_sources.size();




                                    }
                                }
                            }
#endif
                            //indirect

                            #if USE_FINAL_GATHERING

                            Vec3 tmp_N = t_inter.computeNormal();
                            Vec3 rebond = get_random_dir_in_hemisphere(tmp_N);

                            Rayon r_light(inter,rebond);
                            Triangle tmp_tri;
                            Vec3 tmp_inter;
                            if(grille.intersec_ray(r_light,tmp_tri,tmp_inter,t_inter.index)){
                                float radius = 1;
                                PhotonMap voisins = findPhotonVoisin(tmp_inter,pm,radius);

                                Vec3 N = tmp_tri.computeNormal();
                                Vec3 result(0,0,0);
                                double sum = 0;
                                for(auto& v:voisins){
                                    //Vec3 N = v->triangle.computeNormal();
                                    float A = gaussian(v->position-tmp_inter,1);
                                    float cos_theta = glm::dot(-v->dirOrigin,N);
                                    //cos_theta = fabs(cos_theta);
                                    Vec3 Kd_normalize = Vec3(255*v->triangle.Kd.r,255*v->triangle.Kd.g,255*v->triangle.Kd.b)/(float)M_PI;
                                    result += cos_theta*(float)v->energy*Kd_normalize*A;
                                    sum+=A;


                                }
                                Image[i][j] += result / (float)(sum*M_PI*radius);
                            }

                            #else
                            float radius = 1;
                            PhotonMap voisins = findPhotonVoisin(inter,pm,radius);

                            Vec3 N = t_inter.computeNormal();
                            Vec3 result(0,0,0);
                            double sum = 0;
                            for(auto& v:voisins){
                                float A = gaussian(v->position-inter,1);
                                float cos_theta = glm::dot(-v->dirOrigin,N);
                                Vec3 Kd_normalize = Vec3(255*v->triangle.Kd.r,255*v->triangle.Kd.g,255*v->triangle.Kd.b)/(float)M_PI;
                                result += cos_theta*(float)v->energy*Kd_normalize*A;
                                sum+=A;
                            }
                            Image[i][j] += result / (float)(sum*M_PI*radius);
                            #endif


                        }else{
                            Image[i][j] += Vec3(Background_color.red(),Background_color.green(),Background_color.blue());
                        }


                }
                Image[i][j] /= RAY_PIXELS*RAY_PIXELS;

            }
        }
    };
    while(need_thread){
        if(debut + PAS_COMPUTE >= Image.size()){
            fin = Image.size();
            need_thread = false;
        }else{
            fin = debut + PAS_COMPUTE;
        }

        QFuture<void> t = QtConcurrent::run(aux,debut,fin);
        debut = fin;
        liste_threads.push_back(t);
    }

    int nb_threads = liste_threads.size();
    int nb_threads_ended = 0;
    while(nb_threads_ended != nb_threads){
        nb_threads_ended = 0;
        for(auto& t:liste_threads){
            if(t.isFinished()){
                nb_threads_ended++;

            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        emit update_draw();
    }
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

Vec3 ray_photonmapping::get_random_dir_in_hemisphere(Vec3& normal)const{
    Vec3 tmp(1,0,0);
    if(1-fabs(normal.x)<0.001f)
        tmp = Vec3(0,1,0);
    Vec3 u = glm::cross(Vec3(1,0,0),normal);
    Vec3 v = glm::cross(u,normal);


    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_real_distribution<float> distribution(0.0f,1.0f);

    double alpha = 2*M_PI*distribution(generator);
    double betha = acos(1-2*distribution(generator));

    return (float)std::sin(betha)*(u*(float)cos(alpha) + v*(float)sin(alpha)) + normal*(float)cos(betha);
}
