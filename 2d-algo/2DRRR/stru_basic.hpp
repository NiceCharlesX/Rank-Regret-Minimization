#pragma once
#include <array>
#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <iomanip>
#include <algorithm>
#include <fstream>

#define CONVERTER 1000000

using namespace std;

using RankR = int;
using PointId = int;

using Coord = int; //or long? Value
using Point = array<Coord, 2>;
using Dataset = vector<Point>; // 不更新

using PointRankMap = vector<RankR>; // from pointid to rank (不断更新)
using RankPointMap = vector<PointId>; // from rank to pointid (不断更新)
using Solution = vector<PointId>;

using Angle = double;
struct AngleCover{
    PointId pid;
    Angle start;
    Angle end;
};
using AngleCoverQ = vector<AngleCover>;

/**
 * Intersections
**/

struct Intersection{
    Angle angle;
    PointId i;
    PointId j; // guarantee that i < j
    //determine if intersection < based on xcoord, for sort in set
    bool operator < (Intersection const& other) const{
        if(angle == other.angle){
            if(min(i, j) == min(other.i, other.j)){
                return max(i, j) < max(other.i, other.j);
            }
            return min(i, j) < min(other.i, other.j); //determine if PointId < based on the smaller one
        }
        return angle < other.angle;
    }
    bool operator == (Intersection const& other) const{
        return angle == other.angle
            && ((i == other.i && j == other.j) || (j == other.i && i == other.j));
    }
};
/**
 * using an ordered set instead of a vector/heap because we need to check whether a given
 * intersection has already been inserted)
**/
using IntersectionQ = set<Intersection>;//set auto sorted
