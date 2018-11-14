#include "cell.h"



Cell Cell::get_AABB(const List_triangle &list){
    std::vector<double> vect_x;
    std::vector<double> vect_y;
    std::vector<double> vect_z;

    std::for_each(list.begin(),list.end(),[&](const Triangle& t){
        vect_x.push_back(t.s_1.x);
        vect_y.push_back(t.s_1.y);
        vect_z.push_back(t.s_1.z);

        vect_x.push_back(t.s_2.x);
        vect_y.push_back(t.s_2.y);
        vect_z.push_back(t.s_2.z);

        vect_x.push_back(t.s_3.x);
        vect_y.push_back(t.s_3.y);
        vect_z.push_back(t.s_3.z);

    });

    auto minMax_x = std::minmax_element(vect_x.begin(), vect_x.end());

    auto minMax_y = std::minmax_element(vect_y.begin(), vect_y.end());

    auto minMax_z = std::minmax_element(vect_z.begin(), vect_z.end());

    double min_x = minMax_x.first - vect_x.begin();
    double min_y = minMax_y.first - vect_y.begin();
    double min_z = minMax_z.first - vect_z.begin();

    double max_x = minMax_x.second - vect_x.begin();
    double max_y = minMax_y.second - vect_y.begin();
    double max_z = minMax_z.second - vect_z.begin();


    return Cell(min_x,min_y,min_z,max_x,max_y,max_z);
}

bool Cell::intersect_plane(const Vec3& normal,const Vec3& p){
    int q;

    double v;
    Vec3 vmin,vmax;

    Vec3 maxbox = get_half_size();

    for(q=0;q<=2;q++)

    {

        v=p[q];

        if(normal[q]>0.0f)

        {

            vmin[q]=-maxbox[q] - v;

            vmax[q]= maxbox[q] - v;

        }

        else

        {

            vmin[q]= maxbox[q] - v;

            vmax[q]=-maxbox[q] - v;

        }

    }

    if(glm::dot(normal,vmin)>0.0f) return false;

    if(glm::dot(normal,vmax)>=0.0f) return true;



    return false;
}

bool Cell::insertect_tri(const Triangle& t){
    Vec3 v0,v1,v2;
    Vec3 center = get_center();
    v0 = t.s_1 - center;
    v1 = t.s_2 - center;
    v2 = t.s_3 - center;

    Vec3 boxhalfsize = get_half_size();
    auto axistest_X01 = [&](double a,double b,double fa,double fb)->bool{
        double p0 = a*v0.y - b*v0.z;
        double p2 = a*v2.y - b*v2.z;
        double min,max;
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
        double rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
        if(min>rad || max<-rad) return false;
        return true;
    };

    auto axistest_X2 = [&](double a,double b,double fa,double fb)->bool{
        double p0 = a*v0.y - b*v0.z;
        double p1 = a*v2.y - b*v2.z;
        double min,max;
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
        double rad = fa * boxhalfsize.y + fb * boxhalfsize.z;
        if(min>rad || max<-rad) return false;
        return true;
    };

    auto axistest_Y02= [&](double a,double b,double fa,double fb)->bool{
        double p0 = -a*v0.y + b*v0.z;
        double p2 = -a*v2.y + b*v2.z;
        double min,max;
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
        double rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
        if(min>rad || max<-rad) return false;
        return true;
    };

    auto axistest_Y1 = [&](double a,double b,double fa,double fb)->bool{
        double p0 = -a*v0.y + b*v0.z;
        double p2 = -a*v1.y + b*v1.z;
        double min,max;
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
        double rad = fa * boxhalfsize.x + fb * boxhalfsize.z;
        if(min>rad || max<-rad) return false;
        return true;
    };

    auto axistest_Z12 = [&](double a,double b,double fa,double fb)->bool{
        double p0 = a*v1.y - b*v1.z;
        double p2 = a*v2.y - b*v2.z;
        double min,max;
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
        double rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
        if(min>rad || max<-rad) return false;
        return true;
    };

    auto axistest_Z0 = [&](double a,double b,double fa,double fb)->bool{
        double p0 = a*v0.y - b*v0.z;
        double p2 = a*v1.y - b*v1.z;
        double min,max;
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
        double rad = fa * boxhalfsize.x + fb * boxhalfsize.y;
        if(min>rad || max<-rad) return false;
        return true;
    };


    Vec3 e0,e1,e2;

    e0 = v1-v0;
    e1 = v2-v1;
    e2 = v0-v2;

    double fex,fey,fez;

    fex = fabs(e0.x);
    fey = fabs(e0.y);
    fez = fabs(e0.z);
    if(!axistest_X01(e0.z,e0.y,fez,fey))
        return false;
    if(!axistest_Y02(e0.z,e0.x,fez,fex))
        return false;
    if(!axistest_Z12(e0.y,e0.x,fey,fex))
        return false;

    fex = fabs(e1.x);
    fey = fabs(e1.y);
    fez = fabs(e1.z);
    if(!axistest_X01(e1.z,e1.y,fez,fey))
        return false;
    if(!axistest_Y02(e1.z,e1.x,fez,fex))
        return false;
    if(!axistest_Z0(e1.y,e1.x,fey,fex))
        return false;

    fex = fabs(e2.x);
    fey = fabs(e2.y);
    fez = fabs(e2.z);
    if(!axistest_X2(e2.z,e2.y,fez,fey))
        return false;
    if(!axistest_Y1(e2.z,e2.x,fez,fex))
        return false;
    if(!axistest_Z12(e2.y,e2.x,fey,fex))
        return false;

    auto min_max = std::minmax({v0.x,v1.x,v2.x});

    if(min_max.first>boxhalfsize.x || min_max.second<-boxhalfsize.x) return false;

    min_max = std::minmax({v0.y,v1.y,v2.y});

    if(min_max.first>boxhalfsize.y || min_max.second<-boxhalfsize.y) return false;

    min_max = std::minmax({v0.z,v1.z,v2.z});

    if(min_max.first>boxhalfsize.z || min_max.second<-boxhalfsize.z) return false;

    return intersect_plane(t.computeNormal(),v0);
}


List_triangle Cell::triangule()const{
    Vec3 p1 = Vec3(_xmin,_ymin,_zmin);
    Vec3 p2 = Vec3(_xmax,_ymin,_zmin);
    Vec3 p3 = Vec3(_xmax,_ymax,_zmin);
    Vec3 p4 = Vec3(_xmin,_ymax,_zmin);

    Vec3 p5 = Vec3(_xmin,_ymin,_zmax);
    Vec3 p6 = Vec3(_xmax,_ymin,_zmax);
    Vec3 p7 = Vec3(_xmax,_ymax,_zmax);
    Vec3 p8 = Vec3(_xmin,_ymax,_zmax);

    List_triangle lt;

    lt.push_back(Triangle(p1,p2,p3));
    lt.push_back(Triangle(p1,p4,p3));

    lt.push_back(Triangle(p1,p5,p8));
    lt.push_back(Triangle(p1,p4,p8));

    lt.push_back(Triangle(p1,p2,p6));
    lt.push_back(Triangle(p1,p5,p6));

    lt.push_back(Triangle(p7,p8,p4));
    lt.push_back(Triangle(p7,p3,p4));

    lt.push_back(Triangle(p7,p8,p5));
    lt.push_back(Triangle(p7,p6,p5));

    lt.push_back(Triangle(p7,p6,p2));
    lt.push_back(Triangle(p7,p3,p2));

    std::cout << p1 << std::endl;
    std::cout << p7 << std::endl;
    return lt;
}
