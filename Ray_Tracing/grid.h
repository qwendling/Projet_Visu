#ifndef GRID_H
#define GRID_H

#include <vector>
#include <algorithm>
#include "triangle.h"
#include "list_triangle.h"
#include "cell.h"
#include "rayon.h"


class Grid
{
protected:

    Cell aabb;
    Vec3 step_x;
    Vec3 step_y;
    Vec3 step_z;
    unsigned N;
public:
    List_triangle debug_list;
    std::vector<std::vector<std::vector<Cell>>> liste_cell;
    Grid()=delete;
    Grid(List_triangle& list, unsigned def);

    bool intersec_ray(const Rayon& r,Triangle& t,Vec3& inter);
    //pour le debug
    bool intersec_ray(const Rayon& r,Triangle& t,Vec3& inter,std::vector<Cell>& cell_passed);
};

#endif // GRID_H
