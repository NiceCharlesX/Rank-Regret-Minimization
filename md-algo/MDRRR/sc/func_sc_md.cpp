#include "../stru.hpp"

void initialAllWeights(map<PointId, int> & weights){
    for(map<PointId,int>::iterator it = weights.begin(); it != weights.end(); it++){
        it->second = 1;
    }
}

void doubleSetWeights(KSet const& kset, map<PointId, int> & weights){
    for(PointId const& pid: kset){
        weights[pid] *= 2;
    }
}

int getTotalWeight(map<PointId, int> const& weights){
    int tw = 0;
    for(auto const& w: weights) tw += w.second;
    return tw;
}

int getNormalizedWeight(int const& w, int const& cn, int const& tw){
    return w * cn/tw + 1;
}

int getTotalNormalizedWeight(map<PointId, int> const& weights, int const& cn, int const& tw){
    int tnw = 0;
    for(auto const& w: weights) tnw += getNormalizedWeight(w.second, cn, tw);
    return tnw;
}

int getSetNormalizedWeight(KSet const& kset, map<PointId, int> & weights, int const& cn, int const& tw){
    int snw = 0;
    for(PointId pid: kset) snw += getNormalizedWeight(weights[pid], cn, tw);
    return snw;
}

//re为空, 代表所有的kset都被覆盖, 否则返回一个未被覆盖的kset
KSet isSolutionCoveringAllKsets(Solution const& sol, KSets const& ksets){
    for(KSet const& kset : ksets){
        bool isnotcovered = true;
        for(PointId const& pid: kset){
            if(sol.count(pid) == 1){
                isnotcovered = false;
                break;
            }
        }
        if(isnotcovered) return kset;
    }
    return KSet();
}

Solution SetCoverTheo(KSets const& ksets, int const& n){
    map<PointId, int> weights; //存储当前每个point的权重
    for(KSet const& kset : ksets){
        for(PointId const& pid : kset){
            if(weights.count(pid) == 0) weights.insert({pid, 1}); //初始化权重为1
        }
    }
    int cn = weights.size(); //实际只有weights.size()个元祖参与了ksets

    int c = 1;
    Solution sol;
    bool isAllCovered = false;
    do{
        initialAllWeights(weights);
        for(int i = 0; i < 4*c*log2(cn/c); i++){ 
            int tw = getTotalWeight(weights);
            int tnw = getTotalNormalizedWeight(weights, cn, tw);
            KSets reserved_ksets, deleted_ksets;
            for(KSet const& kset: ksets){
                if(getSetNormalizedWeight(kset, weights, cn, tw) >= tnw/4/c){ //构建一个 1/4c-net
                    reserved_ksets.insert(kset);
                }else deleted_ksets.insert(kset);
            }
            sol = SetCover3(reserved_ksets, n);
            KSet uncovered_kset = isSolutionCoveringAllKsets(sol, deleted_ksets);
            if(uncovered_kset.size() == 0){
                isAllCovered = true;
                cout << "cn: " << cn << ", c: " << c << ", times: " << i + 1 << "\n";
                break;
            }else doubleSetWeights(uncovered_kset, weights);
        }
        c *= 2;
    }while(isAllCovered == false);
    return sol;
}

