#include "stru_basic.hpp"

void initializePointers(int const& n, PointRankMap & pointers, RankPointMap & rankpointers){
    for(int i = 0; i < n; i++){
        pointers.push_back(i); //each point has the pointer from id to its rank now
        rankpointers.push_back(i); //each point has the pointer from rank to its id now
	}
}

//find skyline points and mark with pointers in skypointers
int computeSkyline(int const& n, Dataset & points, SkylinePointMap & skypointers){
    Coord MaxX = 0; //当前最大的x坐标值
    int s = 0; //the number of skyline tuples
    for(int i = 0; i < n; i++){
        if(points[i].coords[0] > MaxX){
            skypointers.push_back(i);
            MaxX = points[i].coords[0];
            points[i].sid = s;
            s++;
        }
	}
    return s;
}

//initialize Matrix
//mat[i][j], i代表以第i个skyline元组结尾, j代表容量为j
void initializeMatrix(int const& s, int const& r, vector<vector<Solution>> & mat, 
    PointRankMap const& pointers, SkylinePointMap const& skypointers)
{
    for(int i = 0; i < s; i++){
        vector<Solution> row;
        vector<SkylineId> sids;
        sids.push_back(i);
        for(int j = 0; j < r; j++) row.push_back({sids, pointers[skypointers[i]] + 1});
        mat.push_back(row);
	}
}

//initialize the inter_q with the intersections of all adjacent points
void initializeIntersectionQueue(int const& n, Dataset const& points, IntersectionQ & inter_q){
    for(int i = 1; i < n; i++) add_single_intersection(inter_q, points, i-1, i, 0);
}

/**
 * 初始的排序实现, 按照第二个属性降序排序
**/

//自定义比较器, 按照第二个属性的降序排序
struct compare_points_by_ycoord{
    bool operator() (Point const& p, Point const& q) const{
        if(p.coords[1] == q.coords[1]) return p.coords[0] > q.coords[0];
        return p.coords[1] > q.coords[1];
    }
};

//使用自定义比较器的排序函数
void sort_points(Dataset & points){
    sort(points.begin(), points.end(), compare_points_by_ycoord());
}
