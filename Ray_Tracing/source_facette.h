#ifndef SOURCE_FACETTE_H
#define SOURCE_FACETTE_H
#include "triangle.h"
#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#define ECHANTILLON 200

class source_facette : public Triangle
{
public:
    std::vector<Vec3> liste_sources;
    source_facette(const Triangle &t);
    void draw(GLfloat r,GLfloat g,GLfloat b);
    void move_facette(const Vec3& v);
    Vec3 get_cm(){return (s_1+s_2+s_3)/3.0f;}
};

#endif // SOURCE_FACETTE_H
