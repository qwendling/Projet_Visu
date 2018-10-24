#include "rayon.h"

Rayon::Rayon():origine(0,0,0),direction(0,0,0)
{

}


bool Rayon::intersecTri(Triangle t, Vec3& inter){
    // recuperation des indices de points
    // recuperation des points

    // calcul de l'equation du plan (N+d)

    // calcul de l'intersection rayon plan
    // I = P + alpha*Dir est dans le plan => calcul de alpha

    // alpha => calcul de I

    // I dans le quad ?



    Vec3 p1 = t.s_1;

    Vec3 NQ = t.normal;
    Vec3 P = origine;
    Vec3 Dir = direction;


    float d = -(NQ.x * p1.x + NQ.y * p1.y + NQ.z * p1.z);

    float alpha = ((-d)-(NQ.x*P.x + NQ.y*P.y + NQ.z*P.z))/(NQ.x*Dir.x + NQ.y*Dir.y + NQ.z*Dir.z);
    inter = Dir;
    inter.x *= alpha;
    inter.y *= alpha;
    inter.z *= alpha;

    inter += P;

    return t.is_points_in_triangle(inter);

}
