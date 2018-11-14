#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <matrices.h>
#include <assimp/types.h>

typedef glm::vec3 Vec3;

class Triangle
{
protected:


public:
    Vec3 normal;
    Vec3 s_1;
    Vec3 s_2;
    Vec3 s_3;
    aiColor3D color;

    Triangle(Vec3 s1, Vec3 s2, Vec3 s3);
    Triangle();

    Vec3 computeNormal()const;
    bool is_points_in_triangle(const Vec3& P)const;
    void debugTriangle();

};

#endif // TRIANGLE_H
