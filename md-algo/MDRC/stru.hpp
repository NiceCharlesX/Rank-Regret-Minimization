#pragma once
#include <vector>
#include <iostream>
#include <algorithm> // is_permutation
#include <set>
#include <array>
#include <map>
#include <string>
#include <random>
#include <sys/time.h>
#include <iomanip>
#include <cmath>
#include <fstream>

#define SSV 100000 //Sample size for verification
#define CONVERTER 1000000

using namespace std;

using RankR = int; //rank regret
using PointId = int;
using Coord = int; //or long? Value
using Weight = long double;
using Utility = long double;

using Point = vector<Coord>; 
using Dataset = vector<Point>;

using KSet = set<PointId>;

using UtilityVector = vector<Weight>;
using Solution = set<PointId>;

struct UtilityItem{
    PointId pid;
    Utility uti;
};
using UtilityItems = vector<UtilityItem>;

//未定
using IntervalId = long long;
