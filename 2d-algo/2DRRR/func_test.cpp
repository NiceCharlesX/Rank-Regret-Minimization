#include "stru_basic.hpp"

bool testRankPointer(int const& n, Dataset const& points, RankPointMap const& rankpointers){
    for(int i = 0; i < n-1; i++){
        if(points[rankpointers[i]][1] < points[rankpointers[i + 1]][1]){
            cout << "error 1\n";
            return false;
        }
    }
    return true;
}

//用到了func_initial和func_inter中的函数
bool testResultRankR(int const& n, int const& k, Dataset const& points, Solution const& sol){
    IntersectionQ inter_q; //存储交叉点
    PointRankMap pointers; //each point has the pointer from pointid to its rank now
    RankPointMap rankpointers; //rank i tuple has a rankpointer to its pointid

    initializePointers(n, pointers, rankpointers);
    initializeIntersectionQueue(n, points, inter_q);

    RankR maxminR = 0;
    while(!inter_q.empty()){
        Intersection const inter = pop_front_off_queue(inter_q);
        swap(pointers, rankpointers, inter);
        add_intersections(n, inter_q, points, pointers, rankpointers, inter);
        
        RankR minR = n;
        for(PointId pid: sol) minR = min(minR, pointers[pid] + 1);
        maxminR = max(maxminR, minR);
    }
    cout << "real rank-regret: " << maxminR << "\n";
    if(maxminR < k){
        cout << "result error\n";
        return false;
    }else return true;
}