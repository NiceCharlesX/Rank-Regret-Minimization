#include "stru_basic.hpp"

//自定义比较器, 按照第一个属性的降序排序
struct compare_points_by_xcoord{
    bool operator() (Point const& p, Point const& q) const{
        if(p[0] == q[0]){
            return p[1] > q[1];
        }
        return p[0] > q[0];
    }
};
//使用自定义比较器的排序函数
void sort_points(Dataset & points){
    sort(points.begin(), points.end(), compare_points_by_xcoord());
}

//自定义比较器, 按照cover的start angle的升序排序
struct compare_covers_by_begin{
    bool operator() (AngleCover const& ac1, AngleCover const& ac2) const{
        if(ac1.start == ac2.start){
            return ac1.end < ac2.end;}
        return ac1.start < ac2.start;
    }
};
//使用自定义比较器的排序函数
void sort_covers(AngleCoverQ & angle_cq){
    sort(angle_cq.begin(), angle_cq.end(), compare_covers_by_begin());
}