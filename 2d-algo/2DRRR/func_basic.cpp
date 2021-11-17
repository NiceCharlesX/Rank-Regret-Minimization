#include "stru_basic.hpp"

void initializePointers(int const& n, PointRankMap & pointers, RankPointMap & rankpointers){
    for(int i = 0; i < n; i++){
        pointers.push_back(i);
        rankpointers.push_back(i);
	}
}

void initializeCovers(int const& n, int const& k, AngleCoverQ & angle_cq){
    for(int i = 0; i < k; i++) angle_cq.push_back({i, 0, -1});
    for(int i = k; i < n; i++) angle_cq.push_back({i, -1, -1});
}

void initializeIntersectionQueue(int const& n, Dataset const& points, IntersectionQ & inter_q){
    for(int i = 1; i < n; i++) add_single_intersection(inter_q, points, i-1, i, 0);
}

/**
 * interval range cover algorithm
**/
Solution rangeCover(int const& n, Dataset const & points, AngleCoverQ & angle_cq, Angle const & ang_max){
    sort_covers(angle_cq);
    
    Solution sol;
    PointId s_cid = 0;//start cover_id
    while(angle_cq[s_cid].start < 0) s_cid += 1; //遍历那些无效的cover, i.e., .start = -1

    Angle unc_b = 0; // the begin of the uncovered interval
    while(unc_b < ang_max){
        PointId c_cid; 
        Angle max_e = 0; // the max of end angle of covers
        for(PointId i = s_cid; i < n; i++){
            if(angle_cq[i].start <= unc_b){
                if(angle_cq[i].end > max_e){
                    max_e = angle_cq[i].end;
                    c_cid = i;
                }
            }else break;
        }
        unc_b = angle_cq[c_cid].end;
        sol.push_back(angle_cq[c_cid].pid);
        s_cid = c_cid + 1;
    }
    return sol;
}

