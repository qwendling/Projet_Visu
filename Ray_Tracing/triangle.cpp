#include "triangle.h"

triangle::triangle(Vec3 s1, Vec3 s2, Vec3 s3)
{
    this->s_1 = s1;
    this->s_2 = s2;
    this->s_3 = s3;

    this->normal = computeNormal();
}


Vec3 triangle::computeNormal(){
    Vec3 v12 = s_2-s_1;
    Vec3 v13 = s_3-s_1;

    Vec3 normal = vec_cross(v12,v13);

    return normal / normal.length();
}
