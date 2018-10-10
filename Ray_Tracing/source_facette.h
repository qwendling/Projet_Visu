#ifndef SOURCE_FACETTE_H
#define SOURCE_FACETTE_H
#include "triangle.h"
#include <vector>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#define ECHANTILLON 20

class source_facette
{
protected:
    std::vector<Vec3> liste_sources;
public:
    source_facette(const Triangle &t);
};

#endif // SOURCE_FACETTE_H
