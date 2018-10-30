#include "cell.h"



Cell Cell::get_AABB(const List_triangle &list){
    std::vector<double> vect_x;
    std::vector<double> vect_y;
    std::vector<double> vect_z;

    std::for_each(list.begin(),list.end(),[&](const Triangle& t){
        vect_x.push_back(t.s_1.x);
        vect_y.push_back(t.s_1.y);
        vect_z.push_back(t.s_1.z);

        vect_x.push_back(t.s_2.x);
        vect_y.push_back(t.s_2.y);
        vect_z.push_back(t.s_2.z);

        vect_x.push_back(t.s_3.x);
        vect_y.push_back(t.s_3.y);
        vect_z.push_back(t.s_3.z);

    });

    auto minMax_x = std::minmax_element(vect_x.begin(), vect_x.end());

    auto minMax_y = std::minmax_element(vect_y.begin(), vect_y.end());

    auto minMax_z = std::minmax_element(vect_z.begin(), vect_z.end());

    double min_x = minMax_x.first - vect_x.begin();
    double min_y = minMax_y.first - vect_y.begin();
    double min_z = minMax_z.first - vect_z.begin();

    double max_x = minMax_x.second - vect_x.begin();
    double max_y = minMax_y.second - vect_y.begin();
    double max_z = minMax_z.second - vect_z.begin();


    return Cell(min_x,min_y,min_z,max_x,max_y,max_z);
}
