#include "photon.h"
#include "pointcloud.h"

#define K 20

PhotonMap findPhotonVoisin(Vec3 pos, PhotonMap map,float& radius){

    pcl::PointCloud<pcl::PointXYZ> photon_cloud;
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;

    for(Photon* p : map){
        pcl::PointXYZ tmp_p(p->position.x,p->position.y,p->position.z);
        photon_cloud.push_back(tmp_p);
    }

    kdtree.setInputCloud(photon_cloud.makeShared());

    std::vector<int> k_Neighbor_idx;
    std::vector<float> k_Neighbor_dist;

    pcl::PointXYZ actualPhoton(pos.x,pos.y,pos.z);

    kdtree.nearestKSearch(actualPhoton,K,k_Neighbor_idx,k_Neighbor_dist);

    radius = *std::max_element(k_Neighbor_dist.begin(),k_Neighbor_dist.end());

    PhotonMap result;


    for(auto&& i : k_Neighbor_idx){
        result.push_back(map.at(i));
    }
    return result;
}
