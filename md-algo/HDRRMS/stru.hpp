#pragma once
#include <vector>
#include <iostream>
#include <algorithm> // is_permutation
#include <set>
#include <map>
#include <list>
#include <string>
#include <random>
#include <sys/time.h>
#include <iomanip>
#include <fstream>

#define SSV 100000 //Sample size for verification
#define CONVERTER 1000000

using namespace std;

using RankR = int; //rank regret
using PointId = int;
using Coord = int; //or long? Value
using Weight = float;
using Utility = float;

using Point = vector<Coord>; 
using Dataset = vector<Point>;

using KSet = set<PointId>;
using KSets = set<KSet>;

using UtilityVector = vector<Weight>;
using Solution = set<PointId>;

struct UItem{
    PointId pid;
    Utility uti;
};
