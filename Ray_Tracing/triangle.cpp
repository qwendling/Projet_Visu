#include "triangle.h"

Vec3 operator*(const Vec3& v,int x){
    return Vec3(v.x*x,v.y*x,v.z*x);
}

Vec3 operator/(const Vec3& v,int x){
    return Vec3(v.x/x,v.y/x,v.z/x);
}

std::ostream &operator<<(std::ostream &out,const Triangle& t){
    out << '{';
    out << "s_1 : " << t.s_1 << "\n";
    out << "s_2 : " << t.s_2 << "\n";
    out << "s_3 : " << t.s_3 << "\n";
    out << '}';
    return out;
}

int Triangle::nb_tri = 0;

Triangle::Triangle(Vec3 s1, Vec3 s2, Vec3 s3)
{
    this->s_1 = s1;
    this->s_2 = s2;
    this->s_3 = s3;

    this->normal = computeNormal();
    this->index = Triangle::nb_tri++;
}

Triangle::Triangle(){

}

Vec3 Triangle::computeNormal() const{
    Vec3 v12 = s_2-s_1;
    Vec3 v13 = s_3-s_1;

    Vec3 normal = vec_cross(v12,v13);

    return glm::normalize(normal);
}

bool Triangle::is_points_in_triangle(const Vec3& P) const
{
    Vec3 NQ = this->normal;


    Vec3 NP1 = cross(NQ,s_2-s_1);
    Vec3 NP2 = cross(NQ,s_3-s_2);
    Vec3 NP3 = cross(NQ,s_1-s_3);

    bool T1 = dot(P,NP1) > dot(s_1,NP1);
    bool T2 = dot(P,NP2) > dot(s_2,NP2);
    bool T3 = dot(P,NP3) > dot(s_3,NP3);


    return T1 && T2 && T3;
}


void Triangle::debugTriangle(){
    std::cout << s_1 << " | " << s_2 << " | " << s_3 << " | " << std::endl;
}
