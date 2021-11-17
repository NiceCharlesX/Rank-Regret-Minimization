#include "stru.hpp"

KSets initialUItemsLists(vector<vector<UItem>> & uisl, vector<UtilityVector> const& uvs, Dataset const& points, 
    int const& n, Solution const& basis)
{
    KSets ksets;
    for(int i = 0; i < uvs.size(); i++){
        Utility u_bound = -1; //basis中point关于当前效用向量的最大效用值, 可以用于filter
        for(PointId pid: basis) u_bound = max(u_bound, computeUtiliy(points[pid], uvs[i]));

        for(int j = 0; j < n; j++){
            Utility uti = computeUtiliy(points[j], uvs[i]);
            if(uti > u_bound) uisl[i].push_back({j,uti});
        }

        if(uisl[i].size() > 0){
            nth_element(uisl[i].begin(), uisl[i].begin(), uisl[i].end(), compare_for_nth());//[0,0,s], 找到top-1 point
            KSet kset;
            kset.insert(uisl[i][0].pid);
            if(ksets.count(kset) == 0) ksets.insert(kset);
        }
    }
    return ksets;
}

KSets IncreKSetEnum(vector<vector<UItem>> & uisl, int const& k)
{
    KSets ksets;
    for(int i = 0; i < uisl.size(); i++){
        if(uisl[i].size() >= k){//否则这个效用向量此时已经被basis覆盖
            KSet kset;
            nth_element(uisl[i].begin()+k/2, uisl[i].begin()+k-1, uisl[i].end(), compare_for_nth());//[k/2, k-1, s], 前k/2个point已经排好序
            for(int j = 0; j < k; j++) kset.insert(uisl[i][j].pid);
            if(ksets.count(kset) == 0) ksets.insert(kset);
        }
    }
    return ksets;
}

KSets DecreKSetEnum(vector<vector<UItem>> & uisl, int const& k, int const& low, int const& high){
    KSets ksets;
    for(int i = 0; i < uisl.size(); i++){
        if(uisl[i].size() >= k){//否则这个效用向量此时已经被basis覆盖
            KSet kset;
            if(uisl[i].size() > high){//前low-1个point组成top-(low-1), 前high个point组成top-high
                nth_element(uisl[i].begin()+low-1, uisl[i].begin()+k-1, uisl[i].begin()+high, compare_for_nth());//[low-1, k-1, high]
            }else{//前low-1个point组成top-(low-1)
                nth_element(uisl[i].begin()+low-1, uisl[i].begin()+k-1, uisl[i].end(), compare_for_nth());//[low-1, k-1, s]
            }
            for(int j = 0; j < k; j++) kset.insert(uisl[i][j].pid);
            if(ksets.count(kset) == 0) ksets.insert(kset);
        }
    }
    return ksets;
}


map<KSet, int> initialUItemsListsM(vector<vector<UItem>> & uisl, vector<UtilityVector> const& uvs, Dataset const& points, 
    int const& n, Solution const& basis)
{
    map<KSet, int> ksets;
    for(int i = 0; i < uvs.size(); i++){
        Utility u_bound = -1; //basis中point关于当前效用向量的最大效用值, 可以用于filter
        for(PointId pid: basis) u_bound = max(u_bound, computeUtiliy(points[pid], uvs[i]));

        for(int j = 0; j < n; j++){
            Utility uti = computeUtiliy(points[j], uvs[i]);
            if(uti > u_bound) uisl[i].push_back({j,uti});
        }

        if(uisl[i].size() > 0){
            nth_element(uisl[i].begin(), uisl[i].begin(), uisl[i].end(), compare_for_nth());//[0,0,s], 找到top-1 point
            KSet kset;
            kset.insert(uisl[i][0].pid);
            if(ksets.count(kset) == 0) ksets.insert(pair<KSet, int>(kset, 1)); 
            else ksets[kset] += 1;
        }
    }
    return ksets;
}

map<KSet, int> IncreKSetEnumM(vector<vector<UItem>> & uisl, int const& k)
{
    map<KSet, int> ksets;
    for(int i = 0; i < uisl.size(); i++){
        if(uisl[i].size() >= k){//否则这个效用向量此时已经被basis覆盖
            KSet kset;
            nth_element(uisl[i].begin()+k/2, uisl[i].begin()+k-1, uisl[i].end(), compare_for_nth());//[k/2, k-1, s], 前k/2个point已经排好序
            for(int j = 0; j < k; j++) kset.insert(uisl[i][j].pid);
            if(ksets.count(kset) == 0) ksets.insert(pair<KSet, int>(kset, 1)); 
            else ksets[kset] += 1;
        }
    }
    return ksets;
}

map<KSet, int> DecreKSetEnumM(vector<vector<UItem>> & uisl, int const& k, int const& low, int const& high){
    map<KSet, int> ksets;
    for(int i = 0; i < uisl.size(); i++){
        if(uisl[i].size() >= k){//否则这个效用向量此时已经被basis覆盖
            KSet kset;
            if(uisl[i].size() > high){//前low-1个point组成top-(low-1), 前high个point组成top-high
                nth_element(uisl[i].begin()+low-1, uisl[i].begin()+k-1, uisl[i].begin()+high, compare_for_nth());//[low-1, k-1, high]
            }else{//前low-1个point组成top-(low-1)
                nth_element(uisl[i].begin()+low-1, uisl[i].begin()+k-1, uisl[i].end(), compare_for_nth());//[low-1, k-1, s]
            }
            for(int j = 0; j < k; j++) kset.insert(uisl[i][j].pid);
            if(ksets.count(kset) == 0) ksets.insert(pair<KSet, int>(kset, 1)); 
            else ksets[kset] += 1;
        }
    }
    return ksets;
}