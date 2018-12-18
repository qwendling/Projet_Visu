#include "ray_stochastique.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#define RAY_PIXELS 4
#define PAS_COMPUTE 10

std::vector<Vec3> random_pixels(int i,int j){
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0,1.0/(double)RAY_PIXELS);

    std::vector<Vec3> result;

    double i_jittering=i;
    double j_jittering=j;
    for(int n=0;n<RAY_PIXELS;n++){
        for(int n2=0;n2<RAY_PIXELS;n2++){
            Vec3 tmp(i_jittering,j_jittering,0);
            tmp.x += distribution(generator);
            tmp.y += distribution(generator);
            result.push_back(tmp);
            j_jittering += 1.0/(double)RAY_PIXELS;
        }
        j_jittering = j;
        i_jittering += 1.0/(double)RAY_PIXELS;
    }
    return result;
}

void Ray_stochastique::compute()
{
#if 0
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

                                        Image[i][j] += (color_diff + color_spec)/(float)t.liste_sources.size();
                                    }
                                }
                            }

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
        std::this_thread::sleep_for(std::chrono::seconds(3));
        emit update_draw();
    }


#else
    qglviewer::Vec orig;
    qglviewer::Vec dir;
    for(unsigned i=0;i<Image.size();i++){
        for(unsigned j=0;j<Image[i].size();j++){
            Image[i][j] = Vec3(0,0,0);
            std::vector<Vec3> liste_pixel_random = random_pixels(i,j);

            for(unsigned n = 0;n<liste_pixel_random.size();n++){
                    orig = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,0));
                    dir = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,1));

                    Vec3 o (orig[0],orig[1],orig[2]);
                    Vec3 d (dir[0],dir[1],dir[2]);
                    d=d-o;

                    Rayon r (o,d);

                    Triangle t_inter;
                    Vec3 inter;
                    if(grille.intersec_ray(r,t_inter,inter)){

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

                    }else{
                        Image[i][j] += Vec3(Background_color.red(),Background_color.green(),Background_color.blue());
                    }


            }
            Image[i][j] /= RAY_PIXELS*RAY_PIXELS;
        }
        if(i%PAS_COMPUTE == 0)
            emit update_draw();
    }
#endif
    //emit update_draw();
}

Vec3 Ray_stochastique::get_random_dir_in_cone(Vec3 dir,double angle){
    Vec3 u = glm::cross(Vec3(1,0,0),dir);
    Vec3 v = glm::cross(u,dir);


    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f,1.0f);

    double alpha = 2*M_PI*distribution(generator);
    double betha = angle*distribution(generator);

    return (float)std::sin(betha)*(u*(float)cos(alpha) + v*(float)sin(alpha)) + dir*(float)cos(betha);
}


