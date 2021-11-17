#include "stru.hpp"

struct compare_for_heap{
    bool operator() (UItem const& ui1, UItem const& ui2) const{
        return ui1.uti < ui2.uti;
    }
};

struct compare_for_nth{
    bool operator() (UItem const& ui1, UItem const& ui2) const{
        return ui1.uti > ui2.uti;
    }
};

inline 
Utility computeUtiliy(Point const& p, UtilityVector const& uv){
    Utility re = 0;
    for(int i = 0; i < p.size(); i++) re += uv[i] * p[i];
    return re;
}

bool satisfyConstrain(int const& constrain_n, UtilityVector const& uv){
    for(int i = 0; i < constrain_n; i++){
        //if(uv[i] < uv[i+1]) return false;
        if(uv[uv.size()-i-1] < uv[uv.size()-i-2]) return false;
    }
    return true;
}

UtilityVector generateSatisfiedUtilityVector(int const& constrain_n, int const& d, mt19937 & gen, normal_distribution<float> & nd){
    while(true){
        UtilityVector uv(d);
        for(int i = 0; i < d; i++) uv[i] = fabs(nd(gen));
        if(satisfyConstrain(constrain_n, uv)) return uv;
    }
}

vector<UtilityVector> initialAllUtilityVectors(int const& d, int const& uv_n, int const& constrain_n){ //utility vector num
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> nd;

    vector<UtilityVector> uvs(uv_n);
    for(int i = 0; i < uv_n; i++) uvs[i] = generateSatisfiedUtilityVector(constrain_n, d, gen, nd);
    return uvs;
}

RankR testRankRegret(Dataset const& points, Solution const& sol, vector<UtilityVector> const& uvs){
    RankR maxminR = 0;
    for(UtilityVector const& uv: uvs){
        Utility u_bound = -1; //sol中point关于当前效用向量的最大效用值, 可以用于filter
        for(PointId pid: sol) u_bound = max(u_bound, computeUtiliy(points[pid], uv));

        RankR minR = 1;
        for(Point const& point: points){
            Utility uti = computeUtiliy(point, uv);
            if(uti > u_bound) minR += 1;
        }
        maxminR = max(minR, maxminR);
    }
    return maxminR;
}




