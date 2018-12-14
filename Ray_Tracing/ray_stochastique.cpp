#include "ray_stochastique.h"
#include <cstdlib>
#include <ctime>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#define RAY_PIXELS 3
#define PAS_COMPUTE 20

std::vector<Vec3> random_pixels(int i,int j){
    std::srand(std::time(nullptr));
    std::vector<Vec3> result;
    for(int n=0;n<RAY_PIXELS*RAY_PIXELS;n++){
        Vec3 tmp(i,j,0);
        tmp.x += rand()/(float)RAND_MAX;
        tmp.y += rand()/(float)RAND_MAX;
        result.push_back(tmp);
    }
    return result;
}

void Ray_stochastique::compute()
{
#if 1
    bool need_thread = true;
    int debut = 0;
    int fin;
    std::vector<QFuture<void>> liste_threads;
    auto aux = [&](int debut,int fin){
        qglviewer::Vec orig;
        qglviewer::Vec dir;
        for(unsigned i=debut;i<fin;i++){
            for(unsigned j=0;j<Image[i].size();j++){
                Image[i][j] = Vec3(0,0,0);
                std::vector<Vec3> liste_pixel_random = random_pixels(i,j);

                for(unsigned n = 0;n<liste_pixel_random.size();n++){
                        orig = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,-1));
                        dir = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,1));

                        Vec3 o (orig[0],orig[1],orig[2]);
                        Vec3 d (dir[0],dir[1],dir[2]);

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
                                         Vec3 R = glm::reflect(-L,N);

                                        //Vec3 R = glm::normalize(2*cos_theta*N-L);

                                        Vec3 color_diff = Kd_normalize*cos_theta;

                                        //Bon ca marche mais pas ouf #scotch
                                        if(t_inter.Ns < 1)
                                            t_inter.Ns = 1;

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
        //emit update_draw();
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

    for(auto& t:liste_threads){
        t.waitForFinished();
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
                    orig = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,-1));
                    dir = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,1));

                    Vec3 o (orig[0],orig[1],orig[2]);
                    Vec3 d (dir[0],dir[1],dir[2]);

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
                                     Vec3 R = glm::reflect(-L,N);

                                    //Vec3 R = glm::normalize(2*cos_theta*N-L);

                                    Vec3 color_diff = Kd_normalize*cos_theta;

                                    //Bon ca marche mais pas ouf #scotch
                                    if(t_inter.Ns < 1)
                                        t_inter.Ns = 1;

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
        if(i%PAS_COMPUTE == 0)
            emit update_draw();
    }
#endif
    //emit update_draw();
}

void Ray_stochastique::compute_aux(int debut,int fin){
    qglviewer::Vec orig;
    qglviewer::Vec dir;
    for(unsigned i=debut;i<fin;i++){
        for(unsigned j=0;j<Image[i].size();j++){
            Image[i][j] = Vec3(0,0,0);
            std::vector<Vec3> liste_pixel_random = random_pixels(i,j);

            for(unsigned n = 0;n<liste_pixel_random.size();n++){
                    orig = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,-1));
                    dir = camera.unprojectedCoordinatesOf(qglviewer::Vec(liste_pixel_random[n].x,liste_pixel_random[n].y,1));

                    Vec3 o (orig[0],orig[1],orig[2]);
                    Vec3 d (dir[0],dir[1],dir[2]);

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
                                     Vec3 R = glm::reflect(-L,N);

                                    //Vec3 R = glm::normalize(2*cos_theta*N-L);

                                    Vec3 color_diff = Kd_normalize*cos_theta;

                                    //Bon ca marche mais pas ouf #scotch
                                    if(t_inter.Ns < 1)
                                        t_inter.Ns = 1;

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
    emit update_draw();
}
