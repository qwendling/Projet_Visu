#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <matrices.h>

typedef glm::vec3 Vec3;

class triangle
{
protected:
    Vec3 s_1;
    Vec3 s_2;
    Vec3 s_3;

    Vec3 normal;

public:
    triangle(Vec3 s1, Vec3 s2, Vec3 s3);

    Vec3 computeNormal();
};

#endif // TRIANGLE_H
