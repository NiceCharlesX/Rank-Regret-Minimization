#include "stru.hpp"

/*
** 第0层: 初始区间[0,1]的intervalid为1, 其对切线x=0.5的id也为1, 左边界x=0的id为-1, 右边界x=1的id为1;
** 第1层: [0,1]对半分获得两个区间: [0,0.5], intervalid为10(2), 对切线x=0.25的id也为10; [0.5,1], intervalid为11(3), 对切线x=0.75的id也为11; 
** 第2层: [0,0.5]对半分获得两个区间: [0,0.25], intervalid为100(4), 对切线x=0.125的id也为100; [0.25,0.5], intervalid为101(5), 对切线x=0.375的id也为101; 
** 第2层: [0.5,1]对半分获得两个区间: [0.5,0.75], intervalid为110(6), 对切线x=0.625的id也为110; [0.75,1], intervalid为111(7), 对切线x=0.875的id也为111; 
** 具体分析区间左右边界的id; 每个边界的id, 与其作为对切线的区间id相同;
*/

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

//intervalid/2 是上一级区间的id
//如果二进制末尾是0, 则其继承上一区间的左边界, 拥有新右边界(上一区间的二分之一分割线, 拥有id intervalid/2)
//如果二进制末尾是1, 则其继承上一区间的右边界, 拥有新左边界(上一区间的二分之一分割线, 拥有id intervalid/2)
IntervalId getLeftMargin(IntervalId const& intervalid){
    if(intervalid == 1) return -1;
    else if(intervalid%2 == 0) return getLeftMargin(intervalid/2); //末尾为0, 继承旧左界
    else return intervalid/2; //末尾为1, 新左界
}

IntervalId getRightMargin(IntervalId const& intervalid){
    if(intervalid == 1) return 0;
    else if(intervalid%2 == 0) return intervalid/2; //末尾为0, 新右界
    else return getRightMargin(intervalid/2); //末尾为1, 继承旧右界
}

//R是一个d-1维的区间, comb1是一个d-1维的向量
//input: 3, {1,0}, {1,1}
//output: {0,-1}
vector<IntervalId> transform_to_imv_by_comb1(int const& d, vector<int> const& comb1, vector<IntervalId> const& R){
    vector<IntervalId> imv(d-1); //intermediate vector
    for(int i = 0; i < d-1; i++){
        if(comb1[i] == 0) imv[i] = getLeftMargin(R[i]);
        else imv[i] = getRightMargin(R[i]);
    }
    return imv;
}

//div_dim-th angle被对半切, 生成了新的极坐标向量
//R是一个d-1维的区间, comb2是一个d-2维的向量
vector<IntervalId> transform_to_imv_by_comb2(int const& d, int const& div_dim, vector<int> const& comb2, vector<IntervalId> const& R){
    vector<IntervalId> imv(d-1); //intermediate vector
    for(int i = 0; i < div_dim; i++){
        if(comb2[i] == 0) imv[i] = getLeftMargin(R[i]);
        else imv[i] = getRightMargin(R[i]);
    }
    imv[div_dim] = R[div_dim]; //这是R[div_dim]的对半切的界
    for(int i = div_dim + 1; i < d-1; i++){
        if(comb2[i-1] == 0) imv[i] = getLeftMargin(R[i]);
        else imv[i] = getRightMargin(R[i]);
    }
    return imv;
}

Weight getRelatedAngle(IntervalId const& imvw){ //the weight of the immediate vector
    if(imvw == -1) return 0;
    if(imvw == 0) return 1;
    IntervalId ni = imvw; //the number of intervals on current level
    for(IntervalId i = 1; i < sizeof(IntervalId) * 8; i *= 2){
        ni |= ni >> i;
    }
	ni = (ni + 1) >> 1;
    if(ni == 0){
        cout << "\n" << imvw << " error\n";
        exit(0);
    }
    return (imvw % ni + 0.5) / ni * M_PI/2; //对应当前interval的对切线
}


//将中间极坐标向量, 转换成效用向量
UtilityVector transform_imv_to_uv(int const& d, vector<IntervalId> const& imv){
    UtilityVector uv(d); //根据imv获得效用向量, 文章出处: SIGMOD2017
    Weight im_value = 1.0;
    for(int i = d - 1; i > 0; i--){
        uv[i] = im_value * cos(getRelatedAngle(imv[i-1]));
        im_value = im_value * sin(getRelatedAngle(imv[i-1]));
    }
    uv[0] = im_value;
    return uv;
}
