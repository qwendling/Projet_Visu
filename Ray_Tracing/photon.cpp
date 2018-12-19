#include "photon.h"
#include "pointcloud.h"

#define K 100

void findPhotonVoisin(Photon photon, PhotonMap map){

    pcl::PointCloud<pcl::PointXYZ> photon_cloud;
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;

    int i=0;

    for(Photon* p : map){
        PointPhoton tmp_p(p->position.x,p->position.y,p->position.z,i);
        photon_cloud.push_back(tmp_p);
        i++;
    }

    kdtree.setInputCloud(photon_cloud.makeShared());

    std::vector<int> k_Neighbor_idx;
    std::vector<float> k_Neighbor_dist;

    PointPhoton actualPhoton(photon.position.x,photon.position.y,photon.position.z,0);

    kdtree.nearestKSearch(actualPhoton,K,k_Neighbor_idx,k_Neighbor_dist);


    /*for(auto&& i : k_Neighbor_idx){
        photon.listVoisin.push_back(map.at(photon_cloud[i].idPhoton));
    }*/

}
