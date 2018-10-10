#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <matrices.h>

typedef glm::vec3 Vec3;

class Triangle
{
protected:


    Vec3 normal;

public:
    Vec3 s_1;
    Vec3 s_2;
    Vec3 s_3;
    Triangle(Vec3 s1, Vec3 s2, Vec3 s3);

    Vec3 computeNormal();
};

#endif // TRIANGLE_H
