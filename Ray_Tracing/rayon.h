#ifndef RAYON_H
#define RAYON_H
#include "matrices.h"
#include "triangle.h"

class Rayon
{
protected:
    Vec3 origine;
    Vec3 direction;
    Rayon();
public:
    Rayon(Vec3 o,Vec3 d):origine(o),direction(d){}

    bool intersecTri(Triangle t, Vec3 &pts);


};

#endif // RAYON_H
