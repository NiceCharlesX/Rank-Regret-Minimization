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

vector<UtilityVector> initialAllUtilityVectors(int const& d, int const& ssh, int const& gamma, int const& constrain_n){ //utility vector num
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> nd;
    int sd;
    if(gamma == 0) sd = 0;
    else sd = pow(gamma+1,d-1);

    vector<UtilityVector> uvs(ssh);
    for(int i = 0; i < ssh; i++){
        uvs[i] = generateSatisfiedUtilityVector(constrain_n, d, gen, nd);
    }
    
    vector<int> imv(d-1,0); //每个维度取值都是0到gamma
    for(int j = 0; j < sd; j++){//sd=0则会直接结束
        //cout << imv[0] << "," << imv[1] << "," << imv[2] <<"\n";

        UtilityVector uv(d);
        Weight im_value = 1.0;
        for(int i = d - 1; i > 0; i--){
            uv[i] = im_value * cos(imv[i-1] * M_PI/2.0/gamma);
            im_value = im_value * sin(imv[i-1] * M_PI/2.0/gamma);
        }
        uv[0] = im_value;

        if(satisfyConstrain(constrain_n, uv)) uvs.push_back(uv);

        uvs[ssh+j]=uv;
        for(int i = 0; i < d-1; i++){
            if(imv[i]<gamma){
                imv[i] += 1;
                break;
            }else imv[i] = 0;
        }
        
    }
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

void supplementSolution(int const& constrain_n, Dataset const& points, Solution & sol, 
    int const& n, int const& d, int const& r, int const& fk, int const& ssh)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<float> nd;
    
    for(int i = 0; i < ssh; i++){
        if(sol.size() >= r) break;
        UtilityVector uv = generateSatisfiedUtilityVector(constrain_n, d, gen, nd);
        Utility u_bound = -1; //basis中point关于当前效用向量的最大效用值, 可以用于filter
        for(PointId pid: sol) u_bound = max(u_bound, computeUtiliy(points[pid], uv));

        RankR minR = 1;
        Utility max_u = 0; //maximum utility
        PointId max_p = -1; //the pointid of the maximum
        for(int j = 0; j < n; j++){
            Utility uti = computeUtiliy(points[j], uv);
            if(uti > u_bound){
                minR += 1;
                if(uti > max_u){
                    max_u = uti;
                    max_p = j;
                }
            }
        }
        if(minR > fk) sol.insert(max_p); //每次添加top-1 point
    }
}

