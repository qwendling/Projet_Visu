#ifndef LIST_TRIANGLE_H
#define LIST_TRIANGLE_H

#include "triangle.h"
#include "rayon.h"
#include <vector>
#include <algorithm>

class List_triangle : public std::vector<Triangle>
{
public:
    bool is_intersect(const Rayon&r, Triangle &t, Vec3 &inter);
};

#endif // LIST_TRIANGLE_H
