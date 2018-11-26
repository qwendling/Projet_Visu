#include "ray_phong.h"

void Ray_phong::compute_phong(){
    qglviewer::Vec orig;
    qglviewer::Vec dir;
    QPoint point;
    for(int i=0;i<Image.size();i++){
        for(int j=0;j<Image[i].size();j++){
            point.setX(i);
            point.setY(j);
            camera.convertClickToLine(point, orig, dir);

            Vec3 o (orig[0],orig[1],orig[2]);
            Vec3 d (dir[0],dir[1],dir[2]);

            Rayon r (o,d);

            Triangle t_inter;
            Vec3 inter;
            if(grille.intersec_ray(r,t_inter,inter)){
                //std::cout << "color : " << t_inter.color.r << "," << t_inter.color.g << "," << t_inter.color.b << std::endl;
                Image[i][j] = Vec3(0,0,0);
                for(auto& l:liste_lumiere){
                    Rayon r_light(inter,l-inter);
                    Triangle tmp_tri;
                    Vec3 tmp_inter;
                    if(!grille.intersec_ray(r_light,tmp_tri,tmp_inter,t_inter.index)){
                        Image[i][j] += Vec3(255*t_inter.Kd.r,255*t_inter.Kd.g,255*t_inter.Kd.b)/(float)M_PI;
                        Vec3 H = glm::normalize(l-inter);
                        H += glm::normalize(r.get_origine()-inter);
                        H = glm::normalize(H);
                        Vec3 Ks = Vec3(255*t_inter.Ks.r,255*t_inter.Ks.g,255*t_inter.Ks.b)/(float)(t_inter.Ns+2);
                        Image[i][j] += Ks*(float)fabs(pow(glm::dot(H,t_inter.computeNormal()),t_inter.Ns));
                        //std::cout << "color : " <<  Image[i][j] + Ks*(float)pow(glm::dot(H,t_inter.computeNormal()),1.0f) << std::endl;
                        Image[i][j] *= (float)glm::dot(t_inter.computeNormal(),glm::normalize(l-inter));
                        if(Image[i][j].x >= 255 || Image[i][j].y >= 255 || Image[i][j].z >= 255 || Image[i][j].x < 0 || Image[i][j].y < 0 || Image[i][j].z < 0)
                            std::cout << "color : " <<  Image[i][j]<< std::endl;
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
