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

using RankR = int;//rank or rank-regret
using PointId = int;//根据y坐标排序后的位置
using SkylineId = int;//根据y坐标排序后在全体skyline中的位置

using PointRankMap = vector<RankR>; // from pointid to rank now (不断更新)
using SkylinePointMap = vector<PointId>; // from skylineid to pointid (初始化后, 就不再修改)
using RankPointMap = vector<PointId>; // from rank to pointid (不断更新)

using Coord = int; //long //value
using Coords = array<Coord, 2>; 
struct Point{
    SkylineId sid;
    Coords coords;
};
using Dataset = vector<Point>; //存储所有数据点

struct Solution{
    vector<SkylineId> sids; //解是skyline points的子集
    RankR rankr;
};

/**
 * intersection
**/

using xCoord = double; //x坐标值, 正常取值范围是[0,1], 用于扫描

struct Intersection{
    xCoord xcoord;
    PointId i;
    PointId j; // guarantee that i < j
    //determine if intersection < based on xcoord, for sort in set
    bool operator < (Intersection const& other) const{
        if(xcoord == other.xcoord){
            if(min(i, j) == min(other.i, other.j)){
                return max(i, j) < max(other.i, other.j);
            }
            return min(i, j) < min(other.i, other.j); //determine if PointId < based on the smaller one
        }
        return xcoord < other.xcoord;
    }
    bool operator == (Intersection const& other) const{
        return xcoord == other.xcoord 
            && ((i == other.i && j == other.j) || (j == other.i && i == other.j));
    }
};
/**
 * using an ordered set instead of a vector/heap because we need to check whether a given
 * intersection has already been inserted)
**/
using IntersectionQ = set<Intersection>;//set auto sorted
