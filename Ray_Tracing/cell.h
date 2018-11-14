#ifndef CELL_H
#define CELL_H
#include "triangle.h"
#include "list_triangle.h"
#include <vector>
#include <algorithm>

class Cell{
protected:
    double _xmin;
    double _ymin;
    double _zmin;

    double _xmax;
    double _ymax;
    double _zmax;

public:
    Cell(){}
    Cell(double xmin,double ymin,double zmin,double xmax,double ymax,double zmax):_xmin(xmin),_ymin(ymin),_zmin(zmin),_xmax(xmax),_ymax(ymax),_zmax(zmax){}
    Cell(Vec3& min,Vec3& max):Cell(min.x,min.y,min.z,max.x,max.y,max.z){}
    double get_xmin()const{return _xmin;}
    double get_ymin()const{return _ymin;}
    double get_zmin()const{return _zmin;}
    Vec3 get_min()const{return Vec3(_xmin,_ymin,_zmin);}

    double get_xmax()const{return _xmax;}
    double get_ymax()const{return _ymax;}
    double get_zmax()const{return _zmax;}
    Vec3 get_max()const{return Vec3(_xmax,_ymax,_zmax);}

    static Cell get_AABB(const List_triangle &list);
    bool insertect_tri(const Triangle& t);
    bool intersect_plane(const Vec3& normal,const Vec3& p);
    inline Vec3 get_center()const{Vec3 v = get_min()+get_max();return Vec3(v.x/2,v.y/2,v.z/2);}
    inline Vec3 get_half_size()const{return get_max()-get_center();}
    List_triangle t_list;
    List_triangle triangule()const;

};

#endif // CELL_H
