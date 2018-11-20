#ifndef RAYON_H
#define RAYON_H
#include "matrices.h"
#include "triangle.h"
#include "list_triangle.h"

class Rayon
{
protected:
    Vec3 origine;
    Vec3 direction;
    Rayon();
public:
    Rayon(Vec3 o,Vec3 d):origine(o),direction(d){}
    Rayon(const Rayon& r):origine(r.get_origine()),direction(r.get_direction()){}

    bool intersecTri(const Triangle t, Vec3 &inter)const;
    bool intersecListeTri(List_triangle &liste, Triangle& t_inter, Vec3& p_inter)const;
    Vec3 get_origine()const{return origine;}
    Vec3 get_direction()const{return direction;}
    double dist(const Vec3& v)const;

};

class compare_intersection_foncteur{
private:
    Rayon _r;
public:
    compare_intersection_foncteur() = delete;
    compare_intersection_foncteur(const Rayon& r):_r(r){}
    bool operator()(const std::pair<Triangle,Vec3>& a,const std::pair<Triangle,Vec3>& b)const{
        return (*this)(a.second,b.second);
    }
    bool operator()(const Vec3& a,const Vec3& b)const{
        return _r.dist(a) < _r.dist(b);
    }

};

#endif // RAYON_H
