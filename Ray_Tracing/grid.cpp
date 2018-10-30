#include "grid.h"


Grid::Grid(List_triangle &list,unsigned def){
    Cell aabb_list = Cell::get_AABB(list);
    Vec3 max = aabb_list.get_max();

    Vec3 step_x = Vec3(max.x/(double)def,0,0);
    Vec3 step_y = Vec3(0,max.y/(double)def,0);
    Vec3 step_z = Vec3(0,0,max.z/(double)def);
    Vec3 step_global = step_x+step_y+step_z;

    Vec3 xmin = aabb_list.get_min();
    for(;xmin.x<max.x;xmin+=step_x){
        for(Vec3 ymin = xmin;ymin.y<max.y;ymin+=step_y){
            for(Vec3 zmin = ymin;zmin.z<max.z;zmin+=step_z){
                this->liste_cell.push_back(Cell(zmin,zmin+step_global));
            }
        }
    }
}
