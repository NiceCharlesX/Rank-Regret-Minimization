#include "stru.hpp"

KSet TopK_Heap(Dataset const& points, int const& n, int const& k, UtilityVector const& uv){
    vector<UItem> uis(n);
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

KSet TopK_nth(Dataset const& points, int const& n, int const& k, UtilityVector const& uv){
    vector<UItem> uis(n);
    for(int i = 0; i < n; i++){
        uis[i].pid = i;
        uis[i].uti = computeUtiliy(points[i], uv);
    }
    nth_element(uis.begin(), uis.begin()+k-1, uis.end(), compare_for_nth());
    KSet kset;
    for(int i = 0; i < k; i++) kset.insert(uis[i].pid);
    return kset;
}

//k-set enumeration based on the coupon collector’s problem
KSets KSetEnum(int const& constrain_n, Dataset const& points, int const& n, int const& d, int const& k, int const& thres)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> nd;

    KSets ksets;
    int counter = 0;
    int total = 0;
    while(counter < thres){
        KSet kset = TopK_nth(points, n, k, generateSatisfiedUtilityVector(constrain_n, d, gen, nd)); 
        //KSet kset = TopK_Heap(points, n, k, generateSatisfiedUtilityVector(constrain_n, d, gen, nd)); 
        if(ksets.count(kset) == 1) counter += 1;
        else{
            counter = 0;
            ksets.insert(kset);
        }
        total += 1;
        //if(total >= 10 * SSV){
        //    cout << "\ntime out.\n";
        //    exit(0);
        //}
    }
    cout << "k: " << k << ", sample size: " << total << ", nksets: " << ksets.size() << "\n";
    return ksets;
}
