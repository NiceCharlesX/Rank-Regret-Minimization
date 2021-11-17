#include "../stru.hpp"

//第三种方法
//基于双重结构
Solution SetCover3(KSets const& ksets, int const& n){
    Solution sol;
    vector<KSet> cksets; //存储所有的k-sets, 支持随机访问
    vector<set<int>> pcovs(n); //存储每个点参与的k-sets
    vector<bool> uncov(ksets.size(), true); //存储未被覆盖的k-sets
    map<PointId, int> counters; //存储当前每个point能覆盖的未处理的k-sets数量

    int kid = 0;
    for(KSet const& kset : ksets){
        cksets.push_back(kset);
        for(PointId const& pid : kset){
            pcovs[pid].insert(kid);
            if(counters.count(pid) == 0){
                counters.insert({pid, 1});
            }else{
                counters[pid] += 1;
            }
        }
        kid += 1;
    }

    while(counters.size() > 0){
        int mcn = 0; //the maximum number of covered k-sets
        int m_pid = -1; //the pointid of the maximal
        for(auto const& counter: counters){
            if(counter.second > mcn){
                m_pid = counter.first;
                mcn = counter.second;
            }
        }
        for(int const& kid: pcovs[m_pid]){
            if(uncov[kid]){
                uncov[kid] = false;
                for(PointId const& pid: cksets[kid]){
                    if(counters[pid] == 1){
                        counters.erase(pid);
                    }else{
                        counters[pid] -= 1;
                    }
                }
            }
        }
        sol.insert(m_pid);
    }
    return sol;
}