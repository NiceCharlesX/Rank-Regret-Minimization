#include "stru.hpp"

//自定义比较器, 按照第一个属性的降序排序
struct compare_points_by_xcoord{
    bool operator() (Point const& p, Point const& q) const{
        for(int i = 0; i < p.size(); i++){
            if(p[i] == q[i]) continue;
            return p[i] > q[i];
        }
        return p[0] > q[0];
    }
};
//使用自定义比较器的排序函数
void sort_points(Dataset & points){
    sort(points.begin(), points.end(), compare_points_by_xcoord());
}
