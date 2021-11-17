#include "stru.hpp"

//input: 2;
//output: {{0,0}, {1,0}, {0,1}, {1,1}};
//如果dim = 0, 则会返回一个大小为1的com_set, 其中包含一个大小为0的vector;
vector<vector<int>> getBinomiaCombination(int const& dim){
    vector<vector<int>> comb_set;
    for(int i = 0; i < (int)pow(2, dim); i++){
        vector<int> comb(dim); //如果dim = 0, 这是一个空vector
        int comb_id = i;
        for(int j = 0; j < dim; j++){
            comb[j] = comb_id%2; //comb_id的二进制表示的第j位(从0开始)
            comb_id = comb_id/2;
        }
        comb_set.push_back(comb);
    }
    return comb_set;
}

//R是一个d-1维的区间, comb1是一个d-1维的向量
//input: 3, {1,0}, {[0.5,1],[0.5,1]}
//output: {1,0.5}
vector<Weight> transform_to_imv_by_comb1(int const& d, vector<int> const& comb1, vector<array<Weight, 2>> const& R){
    vector<Weight> imv(d-1); //intermediate vector
    for(int i = 0; i < d-1; i++) imv[i] = (1 - comb1[i]) * R[i][0] + comb1[i] * R[i][1]; //comb1[i] = 0, 取值R[i][0]; comb1[i] = 1, 取值R[i][1] 
    //imv * pi 就是d-1个角, 单独对应一个效用向量
    return imv;
}

//div_dim-th angle被对半切, 生成了新的极坐标向量
//R是一个d-1维的区间, comb2是一个d-2维的向量
//input: 4, 1, {1,0}, {[0.5,1],[0.5,1],[0.5,1]}
//output: {1, 0.75, 0.5}
vector<Weight> transform_to_imv_by_comb2(int const& d, int const& div_dim, vector<int> const& comb2, vector<array<Weight, 2>> const& R){
    vector<Weight> imv(d-1); //intermediate vector
    for(int i = 0; i < div_dim; i++) imv[i] = (1 - comb2[i]) * R[i][0] + comb2[i] * R[i][1];
    //comb1[i] = 0, 取值R[i][0]; comb1[i] = 1, 取值R[i][1] 
    imv[div_dim] = (R[div_dim][0] + R[div_dim][1])/2; 
    //div_dim-th angle被对半切
    for(int i = div_dim + 1; i < d-1; i++) imv[i] = (1 - comb2[i-1]) * R[i][0] + comb2[i-1] * R[i][1];
    return imv;
}

//将中间极坐标向量, 转换成效用向量
//input: 4, {1, 0.75, 0.5}
//output: {sin(pi/4)*sin(3pi/8)*sin(pi/2), sin(pi/4)*sin(3pi/8)*cos(pi/2), sin(pi/4)*cos(3pi/8), cos(pi/4)}
UtilityVector transform_imv_to_uv(int const& d, vector<Weight> const& imv){
    UtilityVector uv(d); //根据imv获得效用向量, 文章出处: SIGMOD2017
    Weight im_value = 1.0;
    for(int i = d - 1; i > 0; i--){
        uv[i] = im_value * cos(imv[i-1] * M_PI/2);
        im_value = im_value * sin(imv[i-1] * M_PI/2);
    }
    uv[0] = im_value;
    return uv;
}
