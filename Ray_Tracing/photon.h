#ifndef PHOTON_H
#define PHOTON_H

#include <matrices.h>
#include "triangle.h"
#include <vector>

typedef glm::vec3 Vec3;

class Photon
{
public:

    Vec3& dirOrigin;
    Vec3& position;

    float energy;
    Triangle& triangle;

    std::vector<Photon*> listVoisin;

    Photon(Vec3& dir,Vec3& pos, float e,Triangle& t):dirOrigin(dir),position(pos),energy(e),triangle(t){}

};


using PhotonMap = std::vector<Photon>;

#endif // PHOTON_H
