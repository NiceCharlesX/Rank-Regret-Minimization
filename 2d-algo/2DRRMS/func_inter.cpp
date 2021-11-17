#include "stru_basic.hpp"

/**
 * Given two 2d points, calculates the x-coordinate of the intersection point of their lines. Returns
 * -1 if the lines are parallel (or coincident).
 * y = a[0] * x + a[1] * (1 - x);
 * y = b[0] * x + b[1] * (1 - x);
 */
inline
xCoord line_intersection_xcoord(Coords const a, Coords const b){
    Coord shift = a[1] - a[0] - b[1] + b[0];
    if( shift <= 0 ){ /* parallel or coincident */
        return -1;}
    else{
        Coord diff = a[1] - b[1];//保证a的y坐标一定比b的大
        return (xCoord)diff / shift;
    }
}

//insert a new intersection
void add_single_intersection(IntersectionQ & inter_q, Dataset const & points, PointId const pid, PointId const qid, xCoord const cur_pos){
    if(pid < qid){
        xCoord xcoord = line_intersection_xcoord(points[pid].coords, points[qid].coords);
        if((xcoord > 0) && (xcoord < 1) && (cur_pos <= xcoord)) inter_q.insert({xcoord, pid, qid});
    }
}

Intersection pop_front_off_queue(IntersectionQ & inter_q){
    auto next_inter = *inter_q.begin();
    inter_q.erase(inter_q.begin());
    return next_inter;
}

// Since the ranks update, swaps the position in pointers and rankpointers
void swap(PointRankMap & pointers, RankPointMap & rankpointers, Intersection const& inter){
    swap(pointers[inter.i], pointers[inter.j]);
    if(pointers[inter.i] != pointers[inter.j] + 1) cout << "error ";
    swap(rankpointers[pointers[inter.i]], rankpointers[pointers[inter.j]]);
}

void add_intersections(int const& n, IntersectionQ & inter_q, Dataset const& points, PointRankMap const& pointers, RankPointMap const& rankpointers, Intersection const& prev_inter){
    RankR ranki = pointers[prev_inter.i], rankj = pointers[prev_inter.j];
    if(ranki < n - 1) add_single_intersection(inter_q, points, prev_inter.i, rankpointers[ranki + 1], prev_inter.xcoord);
    if(rankj > 0) add_single_intersection(inter_q, points, rankpointers[rankj - 1], prev_inter.j, prev_inter.xcoord);
}