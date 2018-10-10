#include "source_facette.h"

source_facette::source_facette(const Triangle& t)
{
    Vec3 u1 = t.s_2 - t.s_1;
    Vec3 u2 = t.s_3 - t.s_1;
    for(int i =0;i<ECHANTILLON;i++){
        double a1 = rand()/RAND_MAX;
        double a2 = (rand()/RAND_MAX)*(1-a1);
        this->liste_sources.push_back(t.s_1 + (u1*a1)+(u2*a2));
    }
}
