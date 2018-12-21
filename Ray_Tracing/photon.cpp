#include "photon.h"
#include "pointcloud.h"

#define K 30

PhotonMap findPhotonVoisin(Vec3 pos, PhotonMap map,float& radius){
    std::vector<int> k_Neighbor_idx;
    std::vector<float> k_Neighbor_dist;

    pcl::PointXYZ actualPhoton(pos.x,pos.y,pos.z);

    map.kdtree.nearestKSearch(actualPhoton,K,k_Neighbor_idx,k_Neighbor_dist);

    radius = *std::max_element(k_Neighbor_dist.begin(),k_Neighbor_dist.end());

    PhotonMap result;


    for(auto&& i : k_Neighbor_idx){
        result.push_back(map.at(i));
    }
    return result;
}
