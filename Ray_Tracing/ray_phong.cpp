#include "ray_phong.h"

void Ray_phong::compute_phong(){
    qglviewer::Vec orig;
    qglviewer::Vec dir;
    //QPoint point;
    for(unsigned i=0;i<Image.size();i++){
        for(unsigned j=0;j<Image[i].size();j++){
            /*point.setX(i);
            point.setY(j);
            camera.convertClickToLine(point, orig, dir);*/
            orig = camera.unprojectedCoordinatesOf(qglviewer::Vec(i,j,-1));
            dir = camera.unprojectedCoordinatesOf(qglviewer::Vec(i,j,1));

            Vec3 o (orig[0],orig[1],orig[2]);
            Vec3 d (dir[0],dir[1],dir[2]);

            Rayon r (o,d);

            Triangle t_inter;
            Vec3 inter;
            if(grille.intersec_ray(r,t_inter,inter)){
                Image[i][j] = Vec3(0,0,0);
                for(auto& l:liste_lumiere){
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

                        Image[i][j] += color_diff + color_spec;
                    }
                }

            }else{
                Image[i][j] = Vec3(Background_color.red(),Background_color.green(),Background_color.blue());
            }
        }
        if(i%40 == 0)
            emit update_draw();
    }
    emit update_draw();
}
