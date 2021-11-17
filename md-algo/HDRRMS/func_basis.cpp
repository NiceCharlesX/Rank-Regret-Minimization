#include "stru.hpp"

Solution getBasis(Dataset const& points, int const& n, int const& d){
    Point max_v(d, 0); //maximum value for each dimension
    vector<PointId> max_c(d, -1); //the candidate of the maximum for each dimension
    for(int i = 0; i < n; i++){
        for(int j = 0; j < d; j++){
            if(points[i][j] > max_v[j]){
                max_v[j] = points[i][j];
                max_c[j] = i;
            }
        }
    }
    return Solution(max_c.begin(), max_c.end());
}

//True: q dominates p;
bool isDominated(Point const& p, Point const & q){
    for(int i = 0; i < p.size(); i++){
        if(p[i] > q[i]) return false;
    }
    return true;
}

//这段代码写的就很凌乱
Solution getBasisAndFilter(Dataset& points, int& n, int const& d){
    Dataset filtered_points;
    Solution basis;

    PointId c = 0;
    for(PointId const& pid: getBasis(points, n, d)){
        filtered_points.push_back(points[pid]);
        basis.insert(c);
        c++;
    }
    for(Point const& point: points){
        bool isNotDom = true;
        for(PointId const& pid: basis){
            if(isDominated(point, filtered_points[pid])) isNotDom = false;
        }
        if(isNotDom) filtered_points.push_back(point);
    }
    points = filtered_points;
    n = points.size();
    return basis;
}
