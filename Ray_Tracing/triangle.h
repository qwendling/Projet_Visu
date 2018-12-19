#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <matrices.h>
#include <assimp/types.h>


typedef glm::vec3 Vec3;

class Triangle
{
protected:


public:
    static int nb_tri;
    int index;
    Vec3 normal;
    Vec3 s_1;
    Vec3 s_2;
    Vec3 s_3;
    aiColor3D Kd;
    aiColor3D Ks;
    aiColor3D Ka;
    double Ns;

    Triangle(Vec3 s1, Vec3 s2, Vec3 s3);
    Triangle(const Triangle& t):index(t.index),normal(t.normal),s_1(t.s_1),s_2(t.s_2),s_3(t.s_3),Kd(t.Kd),Ks(t.Ks),Ka(t.Ka),Ns(t.Ns){}
    Triangle();

    Vec3 computeNormal()const;
    bool is_points_in_triangle(const Vec3& P)const;
    void debugTriangle();

    friend std::ostream &operator<<(std::ostream &out,const Triangle& t);

};

#endif // TRIANGLE_H
