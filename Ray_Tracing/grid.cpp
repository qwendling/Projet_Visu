#include "grid.h"


Grid::Grid(List_triangle &list,unsigned def){
    Cell aabb_list = Cell::get_AABB(list);
    std::cout << "AH ! " << aabb_list.get_min() << list.size() << std::endl;
    Vec3 max = aabb_list.get_max();
    N=def;

    step_x = Vec3(max.x/(double)def,0,0);
    step_y = Vec3(0,max.y/(double)def,0);
    step_z = Vec3(0,0,max.z/(double)def);
    Vec3 step_global = step_x+step_y+step_z;

    liste_cell.resize(def);
    for(int i=0;i<def;i++){
        liste_cell[i].resize(def);
        for(int j=0;j<def;j++){
            liste_cell[i][j].resize(def);
        }
    }

    Vec3 xmin = aabb_list.get_min();
    for(int x=0;xmin.x<max.x;xmin+=step_x,x++){
        int y=0;
        for(Vec3 ymin = xmin;ymin.y<max.y;ymin+=step_y,y++){
            int z=0;
            for(Vec3 zmin = ymin;zmin.z<max.z;zmin+=step_z){
                Vec3 zmax = zmin+step_global;
                this->liste_cell[x][y][z] = Cell(zmin,zmax);
            }
        }
    }

    std::for_each(list.begin(),list.end(),[&](Triangle& t){
        for(int i=0;i<def;i++){
            for(int j=0;j<def;j++){
                for(int k=0;k<def;k++){
                    if(this->liste_cell[i][j][k].insertect_tri(t)){
                        liste_cell[i][j][k].t_list.push_back(t);
                    }
                }
            }
        }
    });
    this->aabb = aabb_list;
}

bool Grid::intersec_ray(const Rayon& r,Triangle& t,Vec3& inter){
    Vec3 cube;
    Triangle tr;
    List_triangle tmp = aabb.triangule();
    if(!r.intersecListeTri(tmp,tr,cube)){
        std::cout << "oups" << std::endl;
        return false;
    }

    Vec3 tMax;
    Vec3 tDelta;
    Vec3 step;

    Vec3 pos;

    pos.x = (cube.x-aabb.get_xmin())/step_x.x;
    pos.y = (cube.y-aabb.get_ymin())/step_y.y;
    pos.z = (cube.z-aabb.get_zmin())/step_z.z;

    cube.x = static_cast<int>(pos.x);
    cube.y = static_cast<int>(pos.y);
    cube.z = static_cast<int>(pos.z);

    Vec3 rayDir = r.get_direction();
    tDelta.x = sqrt(1 + (rayDir.y*rayDir.y)/(rayDir.x*rayDir.x) + (rayDir.z*rayDir.z)/(rayDir.x*rayDir.x));
    tDelta.y = sqrt(1 + (rayDir.x*rayDir.x)/(rayDir.y*rayDir.y) + (rayDir.z*rayDir.z)/(rayDir.y*rayDir.y));
    tDelta.z = sqrt(1 + (rayDir.x*rayDir.x)/(rayDir.z*rayDir.z) + (rayDir.y*rayDir.y)/(rayDir.z*rayDir.z));

    if (rayDir.x < 0)
    {
        step.x = -1;
        tMax.x = (pos.x - floor(pos.x)) *  tDelta.x;
    }
    else
    {
        step.x = 1;
        tMax.x = (ceil(pos.x) - pos.x) *  tDelta.x;
    }

    if (rayDir.y < 0)
    {
        step.y = -1;
        tMax.y = (pos.y - floor(pos.y)) *  tDelta.y;
    }
    else
    {
        step.y = 1;
        tMax.y = (ceil(pos.y) - pos.y)  *  tDelta.y;
    }

    if (rayDir.z < 0)
    {
        step.z = -1;
        tMax.z = (pos.z - floor(pos.z))  *  tDelta.z;
    }
    else
    {
        step.z = 1;
        tMax.z = (ceil(pos.z) - pos.z) *   tDelta.z;
    }

    //Second part of 3DDDA Algorithm starts here: let the ray move on until it hits a filled cube



    while (true)
    {
       if (r.intersecListeTri(liste_cell[cube.x][cube.y][cube.z].t_list,t,inter))
       {
          return true;
       }
       if (tMax.x < tMax.y)
       {
          if (tMax.x < tMax.z)
          {
             cube.x += step.x;
             tMax.x = tMax.x + tDelta.x;
          }
          else
          {
             cube.z += step.z;
             tMax.z = tMax.z + tDelta.z;
          }
       }
       else
       {
          if (tMax.y < tMax.z)
          {
             cube.y += step.y;
             tMax.y = tMax.y + tDelta.y;
          }

          else

          {
             cube.z += step.z;
             tMax.z = tMax.z + tDelta.z;
          }
       }
        if(cube.x >= N || cube.y >= N || cube.z >= N){
            std::cout << "oups 2" << std::endl;
            return false;
        }
    }
}
