#include "rayon.h"
#include <algorithm>

Rayon::Rayon():origine(0,0,0),direction(0,0,0)
{

}


bool Rayon::intersecTri(const Triangle t, Vec3& inter) const{
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

double Rayon::dist(const Vec3& v)const{
    return (v-this->get_origine()).x/this->get_direction().x;
}

bool Rayon::intersecListeTri(List_triangle& liste,Triangle& t_inter,Vec3& p_inter)const{
    bool result = false;
    std::vector<std::pair<Triangle,Vec3>> liste_inter;
    Vec3 tmp_inter;
    std::for_each(liste.begin(),liste.end(),[&](Triangle& t){
        if(this->intersecTri(t,tmp_inter)){
            if(this->dist(tmp_inter) > 0){
                liste_inter.push_back(std::make_pair(t,tmp_inter));
                result = true;
            }
        }
    });

    if(result){
        auto inter_pair = std::min_element(liste_inter.begin(),liste_inter.end(),compare_intersection_foncteur(*this));
        t_inter = inter_pair->first;
        p_inter = inter_pair->second;
    }
    return result;
}

bool Rayon::intersecListeTri(List_triangle &liste, Triangle& t_inter, Vec3& p_inter,Vec3& p_last_inter)const{
    bool result = false;
    std::vector<std::pair<Triangle,Vec3>> liste_inter;
    Vec3 tmp_inter;
    std::for_each(liste.begin(),liste.end(),[&](Triangle& t){
        if(this->intersecTri(t,tmp_inter)){
            if(this->dist(tmp_inter) > 0){
                liste_inter.push_back(std::make_pair(t,tmp_inter));
                result = true;
            }
        }
    });

    if(result){
        auto inter_pair = std::min_element(liste_inter.begin(),liste_inter.end(),compare_intersection_foncteur(*this));
        t_inter = inter_pair->first;
        p_inter = inter_pair->second;
        inter_pair = std::max_element(liste_inter.begin(),liste_inter.end(),compare_intersection_foncteur(*this));
        p_last_inter = inter_pair->second;
    }
    return result;
}
