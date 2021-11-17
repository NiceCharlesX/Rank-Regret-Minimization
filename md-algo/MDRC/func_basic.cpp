#include "stru.hpp"

inline 
Utility computeUtiliy(Point const& p, UtilityVector const& uv){
    Utility re = 0;
    for(int i = 0; i < p.size(); i++) re += uv[i] * p[i];
    return re;
}

struct compare_for_heap{
    bool operator() (UtilityItem const& ui1, UtilityItem const& ui2) const{
        return ui1.uti < ui2.uti;
    }
};

struct compare_for_nth{
    bool operator() (UtilityItem const& ui1, UtilityItem const& ui2) const{
        return ui1.uti > ui2.uti;
    }
};

//构建最大堆
KSet TopK_Heap(Dataset const& points, int const& n, int const& k, UtilityVector const& uv){
    UtilityItems uis(n);
    for(int i = 0; i < n; i++){
        uis[i].pid = i;
        uis[i].uti = computeUtiliy(points[i], uv);
    }
    make_heap(uis.begin(), uis.end(), compare_for_heap());
    KSet kset;
    for(int i = 0; i < k; i++){
        kset.insert(uis.front().pid);
        pop_heap(uis.begin(), uis.end(), compare_for_heap());
        uis.pop_back();
    }
    return kset;
}

//使用已有算法
KSet TopK_nth(Dataset const& points, int const& n, int const& k, UtilityVector const& uv){
    UtilityItems uis(n);
    for(int i = 0; i < n; i++){
        uis[i].pid = i;
        uis[i].uti = computeUtiliy(points[i], uv);
    }
    nth_element(uis.begin(), uis.begin()+k-1, uis.end(), compare_for_nth());
    KSet kset;
    for(int i = 0; i < k; i++) kset.insert(uis[i].pid);
    return kset;
}

inline
UtilityVector generateUtilityVector(int const& d, mt19937 & gen, normal_distribution<float> & nd){
    UtilityVector uv(d);
    for(int i = 0; i < d; i++) uv[i] = fabs(nd(gen));
    return uv;
}

vector<UtilityVector> initialAllUtilityVectors(int const& d, int const& uv_n){ //utility vector num
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> nd;

    vector<UtilityVector> uvs(uv_n);
    for(int i = 0; i < uv_n; i++) uvs[i] = generateUtilityVector(d, gen, nd);
    return uvs;
}

RankR testRankRegret(Dataset const& points, Solution const& sol, vector<UtilityVector> const& uvs){
    RankR maxminR = 0;
    for(UtilityVector const& uv: uvs){
        Utility u_bound = -1; //basis中point关于当前效用向量的最大效用值, 可以用于filter
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




