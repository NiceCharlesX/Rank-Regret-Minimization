#include "stru.hpp"

//初始化所有效用向量下的全排序, 计算所以top-1, 作为kset返回
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
            make_heap(uisl[i].begin(), uisl[i].end(), compare_for_heap());//建堆

            KSet kset;
            kset.insert(uisl[i].front().pid);//插入堆顶元素
            pop_heap(uisl[i].begin(), uisl[i].end(), compare_for_heap());//[0,s], 将最大元素放到末尾
            if(ksets.count(kset) == 0) ksets.insert(kset);
        }
    }
    return ksets;
}

//已经获得k/2的kset, 考虑计算k的kset
KSets IncreKSetEnum(vector<vector<UItem>> & uisl, int const& k)
{
    KSets ksets;
    for(int i = 0; i < uisl.size(); i++){
        if(uisl[i].size() >= k){//否则这个效用向量此时已经被basis覆盖
            KSet kset;
            for(int j = 0; j < k/2; j++){
                pop_heap(uisl[i].begin(), uisl[i].end()-k/2-j, compare_for_heap()); //[0,s-k/2-j], j\in[0,k/2-1], 末尾的k/2+j个元素已经排好序
            }
            for(int j = 0; j < k; j++){
                kset.insert(uisl[i][uisl[i].size()-1-j].pid); //{s-1,s-2,...,s-k}, 末尾的k个元素已经排好序
            }
            if(ksets.count(kset) == 0) ksets.insert(kset);
        }
    }
    return ksets;
}

KSets DecreKSetEnum(vector<vector<UItem>> & uisl, int const& k, int const& low, int const& high)
{
    KSets ksets;
    for(int i = 0; i < uisl.size(); i++){
        if(uisl[i].size() >= k){ //否则这个效用向量此时已经被basis覆盖
            KSet kset;
            if(uisl[i].size() < high){ //此时low到high的部分并未排序
                for(int j = low-1; j < k; j++){ //初始时已经有low-1个元素排序好了, j代表以排序的元素数量, j\in[low-1,k-1], 总共需要排序k-low+1个元素
                    pop_heap(uisl[i].begin(), uisl[i].end()-j, compare_for_heap()); //末尾的j个元素已经排好序
                }
            }
            for(int j = 0; j < k; j++){
                kset.insert(uisl[i][uisl[i].size()-1-j].pid); //末尾的(>=k)个元素已经排好序
            }
            if(ksets.count(kset) == 0) ksets.insert(kset);
        }
    }
    return ksets;
}