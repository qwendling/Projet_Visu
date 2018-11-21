#include "grid.h"

Grid::Grid(List_triangle &list,unsigned def){
    Cell aabb_list = Cell::get_AABB(list);
    std::cout << "max " << aabb_list.get_max() << std::endl;
    std::cout << "min " << aabb_list.get_min() << std::endl;

    Vec3 max = aabb_list.get_max();
    Vec3 min = aabb_list.get_min();


    N=def;

    step_x = Vec3((max.x-min.x)/(double)def,0,0);
    step_y = Vec3(0,(max.y-min.y)/(double)def,0);
    step_z = Vec3(0,0,(max.z-min.z)/(double)def);
    Vec3 step_global = step_x+step_y+step_z;

    max = max + (step_global/10.0f);
    min = min - (step_global/10.0f);

    step_x = Vec3((max.x-min.x)/(double)def,0,0);
    step_y = Vec3(0,(max.y-min.y)/(double)def,0);
    step_z = Vec3(0,0,(max.z-min.z)/(double)def);
    step_global = step_x+step_y+step_z;

    liste_cell.resize(def);
    for(int i=0;i<def;i++){
        liste_cell[i].resize(def);
        for(int j=0;j<def;j++){
            liste_cell[i][j].resize(def);
        }
    }

    Vec3 xmin = min;
    for(int x=0;x<def;xmin+=step_x,x++){
        int y=0;
        for(Vec3 ymin = xmin;y<def;ymin+=step_y,y++){
            int z=0;
            for(Vec3 zmin = ymin;z<def;zmin+=step_z,z++){
                Vec3 zmax = zmin+step_global;
                this->liste_cell[x][y][z] = Cell(zmin,zmax);
            }
        }
    }
    std::for_each(list.begin(),list.end(),[&](Triangle& t){
        debug_list.push_back(t);
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
    Vec3 pos;
    Triangle tr;
    List_triangle tmp = aabb.triangule();

    //std::cout << "debut dda" << std::endl;

    if((r.get_origine().x <= this->aabb.get_xmax() && r.get_origine().y <= this->aabb.get_ymax() && r.get_origine().z <= this->aabb.get_zmax())
            && (r.get_origine().x >= this->aabb.get_xmin() && r.get_origine().y >= this->aabb.get_ymin() && r.get_origine().z >= this->aabb.get_zmin())){
        cube = r.get_origine();
    }else{
        if(!r.intersecListeTri(tmp,tr,cube)){
            //std::cout << "dehors " << std::endl;
            return false;
        }
    }

    Vec3 tMax;
    Vec3 tDelta;
    Vec3 step;
    Vec3 origin_r = cube;




    pos.x = N*(cube.x-aabb.get_xmin())/(aabb.get_xmax()-aabb.get_xmin());
    pos.y = N*(cube.y-aabb.get_ymin())/(aabb.get_ymax()-aabb.get_ymin());
    pos.z = N*(cube.z-aabb.get_zmin())/(aabb.get_zmax()-aabb.get_zmin());



    cube.x = static_cast<int>(pos.x);
    cube.y = static_cast<int>(pos.y);
    cube.z = static_cast<int>(pos.z);

    if(cube.x == N){
        cube.x -=1;
    }
    if(cube.y == N){
        cube.y -=1;
    }
    if(cube.z == N){
        cube.z -=1;
    }



    Vec3 rayDir = r.get_direction();

#if 0
    tDelta.x = sqrt(1 + (rayDir.y*rayDir.y)/(rayDir.x*rayDir.x) + (rayDir.z*rayDir.z)/(rayDir.x*rayDir.x));
    tDelta.y = sqrt(1 + (rayDir.x*rayDir.x)/(rayDir.y*rayDir.y) + (rayDir.z*rayDir.z)/(rayDir.y*rayDir.y));
    tDelta.z = sqrt(1 + (rayDir.x*rayDir.x)/(rayDir.z*rayDir.z) + (rayDir.y*rayDir.y)/(rayDir.z*rayDir.z));

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
#else
    Vec3 rayOrigGrid = origin_r-aabb.get_min();
    double voxellSizeX = (aabb.get_xmax()-aabb.get_xmin())/N;
    double voxellSizeY = (aabb.get_ymax()-aabb.get_ymin())/N;
    double voxellSizeZ = (aabb.get_zmax()-aabb.get_zmin())/N;

    if (rayDir.x < 0)
    {
        step.x = -1;
        tDelta.x = - voxellSizeX / r.get_direction().x;
        tMax.x = (floor(rayOrigGrid.x / voxellSizeX) * voxellSizeX - rayOrigGrid.x) / rayDir.x;
    }
    else
    {
        step.x = 1;
        tDelta.x = voxellSizeX / r.get_direction().x;
        tMax.x = (floor(rayOrigGrid.x / voxellSizeX +1) * voxellSizeX - rayOrigGrid.x) / rayDir.x;
    }

    if (rayDir.y < 0)
    {
        step.y = -1;
        tDelta.y = - voxellSizeY / r.get_direction().y;
        tMax.y = (floor(rayOrigGrid.y / voxellSizeY) * voxellSizeY - rayOrigGrid.y) / rayDir.y;
    }
    else
    {
        step.y = 1;
        tDelta.y = voxellSizeY / r.get_direction().y;
        tMax.y = (floor(rayOrigGrid.y / voxellSizeY +1) * voxellSizeY - rayOrigGrid.y) / rayDir.y;
    }

    if (rayDir.z < 0)
    {
        step.z = -1;
        tDelta.z = - voxellSizeZ / r.get_direction().z;
        tMax.z = (floor(rayOrigGrid.z / voxellSizeZ) * voxellSizeZ - rayOrigGrid.z) / rayDir.z;
    }
    else
    {
        step.z = 1;
        tDelta.z = voxellSizeZ / r.get_direction().z;
        tMax.z = (floor(rayOrigGrid.z / voxellSizeZ +1) * voxellSizeZ - rayOrigGrid.z) / rayDir.z;
    }
#endif
    //Second part of 3DDDA Algorithm starts here: let the ray move on until it hits a filled cube



    while (true)
    {
        if(cube.x >= N || cube.y >= N || cube.z >= N || cube.x < 0 || cube.y < 0 || cube.z < 0){
            //std::cout << "rien trouver " << std::endl;
            return false;
        }
       if (r.intersecListeTri(liste_cell[cube.x][cube.y][cube.z].t_list,t,inter))
       {
           //std::cout << "inter find" << std::endl;
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

    }
}
