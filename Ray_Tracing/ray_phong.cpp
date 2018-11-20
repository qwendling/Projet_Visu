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
                Image[i][j] = Vec3(255*t_inter.color.r,255*t_inter.color.g,255*t_inter.color.b);

            }else{
                Image[i][j] = Vec3(Background_color.red(),Background_color.green(),Background_color.blue());
            }
        }
    }
}
