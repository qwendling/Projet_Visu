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
    std::vector<std::vector<std::vector<Cell>>> liste_cell;
    Cell aabb;
    Vec3 step_x;
    Vec3 step_y;
    Vec3 step_z;
    unsigned N;
public:
    Grid()=delete;
    Grid(List_triangle& list, unsigned def);

    bool intersec_ray(const Rayon& r,Triangle& t,Vec3& inter);
};

#endif // GRID_H
