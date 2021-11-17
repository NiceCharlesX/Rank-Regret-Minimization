#include "stru.hpp"

//第三种方法
//基于双重结构
Solution SetCover(KSets const& ksets, int const& n){
    Solution sol;
    vector<KSet> cksets; //存储所有的k-sets, 支持随机访问
    vector<set<int>> pcovs(n); //存储每个点参与的k-sets
    vector<bool> uncov(ksets.size(), true); //存储未被覆盖的k-sets
    map<PointId, int> counters; //存储当前每个point能覆盖的未处理的k-sets数量

    int kid = 0;
    for(KSet const& kset : ksets){
        cksets.push_back(kset); //初始化存储所有kset的数组cksets
        for(PointId const& pid : kset){
            pcovs[pid].insert(kid); //pid可以覆盖当前kid的kset
            if(counters.count(pid) == 0) counters.insert({pid, 1}); //pid之前未出现
            else counters[pid] += 1;
        }
        kid += 1;
    }

    while(counters.size() > 0){
        int mcn = 0, m_pid = -1; 
        //mcn: the maximum number of covered k-sets, m_pid: the pointid of the maximal
        for(auto const& counter: counters){
            if(counter.second > mcn){
                m_pid = counter.first;
                mcn = counter.second;
            }
        }
        for(int const& kid: pcovs[m_pid]){
            if(uncov[kid]){ //当前kid的kset之前未被覆盖
                uncov[kid] = false; //标记为已被覆盖
                for(PointId const& pid: cksets[kid]){ //更新所有元组的counter
                    if(counters[pid] == 1) counters.erase(pid); //当前元组不再覆盖任意的kset
                    else counters[pid] -= 1; 
                }
            }
        }
        sol.insert(m_pid);
    }
    return sol;
}

//加权集合覆盖
Solution SetCoverM(map<KSet, int> const& ksets, int const& n){
    Solution sol;
    vector<KSet> cksets; //存储所有的k-sets, 支持随机访问
    vector<set<int>> pcovs(n); //存储每个点参与的k-sets
    vector<bool> uncov(ksets.size(), true); //存储未被覆盖的k-sets
    map<PointId, int> counters; //存储当前每个point能覆盖的未处理的k-sets数量

    int kid = 0;
    for(auto const& item: ksets){
        cksets.push_back(item.first);
        for(PointId const& pid: item.first){
            pcovs[pid].insert(kid);
            if(counters.count(pid) == 0) counters.insert({pid, item.second});
            else counters[pid] += item.second;
        }
        kid += 1;
    }

    while(counters.size() > 0){
        int mcn = 0, m_pid = -1; //the pointid of the maximal
        for(auto const& counter: counters){
            if(counter.second > mcn){
                m_pid = counter.first;
                mcn = counter.second;
            }
        }
        for(int const& kid: pcovs[m_pid]){
            if(uncov[kid]){
                uncov[kid] = false;
                int kc = ksets.find(cksets[kid])->second; //the number of the same kset
                for(PointId const& pid: cksets[kid]){
                    if(counters[pid] == kc) counters.erase(pid);
                    else counters[pid] -= kc;
                }
            }
        }
        sol.insert(m_pid);
    }
    return sol;
}