#include "cell.h"



Cell Cell::get_AABB(const List_triangle &list){
    std::vector<double> vect_x;
    std::vector<double> vect_y;
    std::vector<double> vect_z;

    double min_x = DBL_MAX;
    double min_y = DBL_MAX;
    double min_z = DBL_MAX;

    double max_x = DBL_MIN;
    double max_y = DBL_MIN;
    double max_z = DBL_MIN;

    std::for_each(list.begin(),list.end(),[&](const Triangle& t){
        if(min_x > t.s_1.x)
            min_x = t.s_1.x;
        if(min_x > t.s_2.x)
            min_x = t.s_2.x;
        if(min_x > t.s_3.x)
            min_x = t.s_3.x;

        if(min_y > t.s_1.y)
            min_y = t.s_1.y;
        if(min_y > t.s_2.y)
            min_y = t.s_2.y;
        if(min_y > t.s_3.y)
            min_y = t.s_3.y;

        if(min_z > t.s_1.z)
            min_z = t.s_1.z;
        if(min_z > t.s_2.z)
            min_z = t.s_2.z;
        if(min_z > t.s_3.z)
            min_z = t.s_3.z;

        if(max_x < t.s_1.x)
            max_x = t.s_1.x;
        if(max_x < t.s_2.x)
            max_x = t.s_2.x;
        if(max_x < t.s_3.x)
            max_x = t.s_3.x;

        if(max_y < t.s_1.y)
            max_y = t.s_1.y;
        if(max_y < t.s_2.y)
            max_y = t.s_2.y;
        if(max_y < t.s_3.y)
            max_y = t.s_3.y;

        if(max_z < t.s_1.z)
            max_z = t.s_1.z;
        if(max_z < t.s_2.z)
            max_z = t.s_2.z;
        if(max_z < t.s_3.z)
            max_z = t.s_3.z;

    });


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

    Vec3 extents = get_half_size();
    /*auto axistest_X01 = [&](double a,double b,double fa,double fb)->bool{
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
    };*/


    Vec3 e0,e1,e2;

    e0 = v1-v0;
    e1 = v2-v1;
    e2 = v0-v2;


    Vec3 a00 = Vec3( 0, -e0.z, e0.y ),
          a01 = Vec3( 0, -e1.z, e1.y ),
          a02 = Vec3( 0, -e2.z, e2.y ),
          a10 = Vec3( e0.z, 0, -e0.x ),
          a11 = Vec3( e1.z, 0, -e1.x ),
          a12 = Vec3( e2.z, 0, -e2.x ),
          a20 = Vec3( -e0.y, e0.x, 0 ),
          a21 = Vec3( -e1.y, e1.x, 0 ),
          a22 = Vec3( -e2.y, e2.x, 0 );

    double p0,p1,p2,r;

    // Test axis a00
      p0 =glm::dot(v0, a00 );
      p1 =glm::dot(v1, a00 );
      p2 =glm::dot(v2, a00 );
      r = extents.y * std::abs( e0.z ) + extents.z * std::abs( e0.y );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a01
      p0 =glm::dot(v0, a01 );
      p1 =glm::dot(v1, a01 );
      p2 =glm::dot(v2, a01 );
      r = extents.y * std::abs( e1.z ) + extents.z * std::abs( e1.y );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a02
      p0 =glm::dot(v0, a02 );
      p1 =glm::dot(v1, a02 );
      p2 =glm::dot(v2, a02 );
      r = extents.y * std::abs( e2.z ) + extents.z * std::abs( e2.y );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a10
      p0 =glm::dot(v0, a10 );
      p1 =glm::dot(v1, a10 );
      p2 =glm::dot(v2, a10 );
      r = extents.x * std::abs( e0.z ) + extents.z * std::abs( e0.x );
      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a11
      p0 =glm::dot(v0, a11 );
      p1 =glm::dot(v1, a11 );
      p2 =glm::dot(v2, a11 );
      r = extents.x * std::abs( e1.z ) + extents.z * std::abs( e1.x );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a12
      p0 =glm::dot(v0, a12 );
      p1 =glm::dot(v1, a12 );
      p2 =glm::dot(v2, a12 );
      r = extents.x * std::abs( e2.z ) + extents.z * std::abs( e2.x );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a20
      p0 =glm::dot(v0, a20 );
      p1 =glm::dot(v1, a20 );
      p2 =glm::dot(v2, a20 );
      r = extents.x * std::abs( e0.y ) + extents.y * std::abs( e0.x );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a21
      p0 =glm::dot(v0, a21 );
      p1 =glm::dot(v1, a21 );
      p2 =glm::dot(v2, a21 );
      r = extents.x * std::abs( e1.y ) + extents.y * std::abs( e1.x );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test axis a22
      p0 =glm::dot(v0, a22 );
      p1 =glm::dot(v1, a22 );
      p2 =glm::dot(v2, a22 );
      r = extents.x * std::abs( e2.y ) + extents.y * std::abs( e2.x );

      if ( std::max( -std::max( {p0, p1, p2} ), std::min( {p0, p1, p2} ) ) > r ) {

        return false; // Axis is a separating axis

      }

      // Test the three axes corresponding to the face normals of AABB b (category 1).
       // Exit if...
       // ... [-extents.x, extents.x] and [min(v0.x,v1.x,v2.x), max(v0.x,v1.x,v2.x)] do not overlap
       if ( std::max({ v0.x, v1.x, v2.x }) < -extents.x || std::min({ v0.x, v1.x, v2.x }) > extents.x ) {

         return false;

       }
       // ... [-extents.y, extents.y] and [min(v0.y,v1.y,v2.y), max(v0.y,v1.y,v2.y)] do not overlap
       if ( std::max({ v0.y, v1.y, v2.y }) < -extents.y || std::min({ v0.y, v1.y, v2.y }) > extents.y ) {

         return false;

       }
       // ... [-extents.z, extents.z] and [min(v0.z,v1.z,v2.z), max(v0.z,v1.z,v2.z)] do not overlap
       if ( std::max({ v0.z, v1.z, v2.z }) < -extents.z || std::min({ v0.z, v1.z, v2.z }) > extents.z ) {

         return false;

       }

/*
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

    if(min_max.first>boxhalfsize.z || min_max.second<-boxhalfsize.z) return false;*/

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

    /*std::cout << p1 << std::endl;
    std::cout << p7 << std::endl;*/
    return lt;
}
