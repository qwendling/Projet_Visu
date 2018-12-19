#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>


class PointPhoton : public pcl::PointXYZ
{
public:

    int idPhoton;

    PointPhoton(float xc, float yc, float zc, int id):pcl::PointXYZ(xc,yc,zc),idPhoton(id){}
};

#endif // POINTCLOUD_H
