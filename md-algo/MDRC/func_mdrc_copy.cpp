#include "stru.hpp"

//计算kset的交, 并将kset1修改为交结果
void kset_intersection(KSet & kset1, KSet const& kset2){
    KSet im_kset;
    set_intersection(kset1.begin(), kset1.end(), kset2.begin(), kset2.end(), inserter(im_kset, im_kset.begin()));
    kset1.clear();
    kset1 = im_kset;
}

//计算kset的并, 并返回
KSet kset_union(KSet const& kset1, KSet const& kset2){
    KSet re;
    set_union(kset1.begin(), kset1.end(), kset2.begin(), kset2.end(), inserter(re, re.begin()));
    return re;
}

Solution MDRC(int const& n, int const& d, int const&k, Dataset const& points, vector<vector<int>> const& comb_set1, vector<vector<int>> const& comb_set2, 
    int const& level, vector<IntervalId> const& R, map<vector<IntervalId>, KSet> & ksets)
{
    KSet is_set = ksets[transform_to_imv_by_comb1(d, comb_set1[0], R)];
    for(int i = 1; i < comb_set1.size(); i++){
        kset_intersection(is_set, ksets[transform_to_imv_by_comb1(d, comb_set1[i], R)]);
        if(is_set.size() == 0) break;
    }
    //计算2^(d-1)个kset的交(所有这些kset已经计算过)

    if(is_set.size() > 0){//交不为空
        Solution sol;
        sol.insert(*is_set.begin());//插入 交集的第一个point 的pointid
        return sol;
    }

    int div_dim = level%(d-1);//当前进行划分的极角坐标
    for(vector<int> const& comb2: comb_set2){
        vector<IntervalId> imv = transform_to_imv_by_comb2(d, div_dim, comb2, R);
        if(ksets.count(imv) == 0){//对应kset还未构造
            KSet kset = TopK_nth(points, n, k, transform_imv_to_uv(d, imv));
            ksets.insert({imv, kset});
        }
    }
    vector<IntervalId> LR = R, RR = R;//构造新的探索region
    if(R[div_dim] >= LLONG_MAX/2){
        cout << R[div_dim] << "\n";
        exit(0);
    }
    LR[div_dim] = R[div_dim] * 2;//划分坐标区间从新构造
    RR[div_dim] = R[div_dim] * 2 + 1;//划分坐标区间从新构造
    return kset_union(MDRC(n, d, k, points, comb_set1, comb_set2, level+1, LR, ksets), MDRC(n, d, k, points, comb_set1, comb_set2, level+1, RR, ksets));
}

Solution initialAndStart(Dataset const& points, int const& n, int const& d, int const& k)
{
    vector<IntervalId> R(d-1, 1);//使用极坐标系, 有d-1个角, 每个intervalid都是1
    vector<vector<int>> comb_set1 = getBinomiaCombination(d - 1); //{0,1}^(d-1)获得的所有d-1维向量, 就是笛卡尔积
    vector<vector<int>> comb_set2 = getBinomiaCombination(d - 2); //{0,1}^(d-2)获得的所有d-2维向量, 就是笛卡尔积
    map<vector<IntervalId>, KSet> ksets;
    for(vector<int> const& comb1: comb_set1){
        vector<IntervalId> imv = transform_to_imv_by_comb1(d, comb1, R);
        KSet kset = TopK_nth(points, n, k, transform_imv_to_uv(d, imv));
        ksets.insert(map<vector<IntervalId>, KSet>::value_type(imv, kset)); //({imv, kset});
    }
    return MDRC(n, d, k, points, comb_set1, comb_set2, 0, R, ksets);
}
