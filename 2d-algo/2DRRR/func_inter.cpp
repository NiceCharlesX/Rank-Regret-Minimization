#include "stru_basic.hpp"

/**
 * 1 = a[0] * x + a[1] * y;
 * 1 = b[0] * x + b[1] * y;
 */
inline
Angle line_intersection(Point const a, Point const b){
    Coord shift = b[1] - a[1];
    if(shift <= 0) return -1; //parallel or have no intersection
    else return (double)(a[0]-b[0]) / shift;
}

void add_single_intersection(IntersectionQ & inter_q, Dataset const & points, PointId const pid, PointId const qid, Angle const cur_pos){
    if(pid < qid){
        Angle angle = line_intersection(points[pid], points[qid]);
        if((angle > 0) && (cur_pos <= angle)) inter_q.insert({angle, pid, qid});
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
    if(pointers[inter.i] != pointers[inter.j]+1) cout << "error ";
    swap(rankpointers[pointers[inter.i]], rankpointers[pointers[inter.j]]);
}

void add_intersections(int const& n, IntersectionQ & inter_q, Dataset const& points, PointRankMap const& pointers, RankPointMap const& rankpointers, Intersection const& prev_inter){
    RankR ranki = pointers[prev_inter.i], rankj = pointers[prev_inter.j];
    if(ranki < n - 1) add_single_intersection(inter_q, points, prev_inter.i, rankpointers[ranki + 1], prev_inter.angle);
    if(rankj > 0) add_single_intersection(inter_q, points, rankpointers[rankj - 1], prev_inter.j, prev_inter.angle);
}
