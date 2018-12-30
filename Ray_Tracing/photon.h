#ifndef PHOTON_H
#define PHOTON_H

#include <matrices.h>
#include "triangle.h"
#include <vector>
#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>


typedef glm::vec3 Vec3;

class Photon
{
public:

    Vec3 dirOrigin;
    Vec3 position;

    float energy;
    Triangle triangle;

    std::vector<Photon*> listVoisin;
    Photon(Vec3& dir,Vec3& pos, float e,Triangle& t):dirOrigin(dir),position(pos),energy(e),triangle(t){}
};

class PhotonMap : public std::vector<Photon*>{
public :
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
};


PhotonMap findPhotonVoisin(Vec3 &pos, PhotonMap &map, float &radius);

#endif // PHOTON_H
