#ifndef GRID_H
#define GRID_H

#include <vector>
#include <algorithm>
#include "triangle.h"
#include "list_triangle.h"
#include "cell.h"


class Grid
{
protected:
    std::vector<Cell> liste_cell;
public:
    Grid()=delete;
    Grid(List_triangle& list, unsigned def);
};

#endif // GRID_H
