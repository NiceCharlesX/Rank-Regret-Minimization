#include "2DRRR/func_io.cpp"
#include "2DRRR/func_inter.cpp"
#include "2DRRR/func_sort.cpp"
#include "2DRRR/func_basic.cpp"
#include "2DRRR/func_test.cpp"

Solution getOptimalSol(int const& n, int const& k, Dataset& points){
    IntersectionQ inter_q; //存储交叉点
    AngleCoverQ angle_cq;
    PointRankMap pointers; //each point has the pointer from pointid to its rank now
    RankPointMap rankpointers; //rank i tuple has a rankpointer to its pointid

    initializePointers(n, pointers, rankpointers);
    initializeCovers(n, k, angle_cq);
    initializeIntersectionQueue(n, points, inter_q);

    Angle ang_max = 0.0;
    while(!inter_q.empty()){
        Intersection const inter = pop_front_off_queue(inter_q);
        if(pointers[inter.i] == k-1){ // pi is ranked as k-th tuple
            if(angle_cq[inter.j].start == -1){
                angle_cq[inter.j].start = inter.angle;
            }
            angle_cq[inter.i].end = inter.angle;
        }
        swap(pointers, rankpointers, inter);
        add_intersections(n, inter_q, points, pointers, rankpointers, inter);
        ang_max = inter.angle + 1;
    }
    
    for(int i = 0; i < k; i++) angle_cq[rankpointers[i]].end = ang_max;
    testRankPointer(n, points, rankpointers);
    return rangeCover(n, points, angle_cq, ang_max);
}


int main(int argc, char *argv[])
{
    int n = 10000, r = 5, fid = 3;
    ofstream RECORD("../record/count.txt", ios::app); //app = append
    if(argc > 1) n = stoi(string(argv[1]));
    if(argc > 2) r = stoi(string(argv[2]));
    if(argc > 3) fid = stoi(string(argv[3]));
    cout<<"\n2DRRR "<<n<<" "<<r<<" "<<fid<<"\n";
    RECORD<<"\n2DRRR "<<n<<" "<<r<<" "<<fid<<"\n";
    Dataset points; // 所有数据第一次排序(按第二个属性降序)后, 就不会更改在points中的顺序
    read_file(points, n, fid);

    struct timeval start, end;
    gettimeofday(&start,NULL);

    sort_points(points);
    int k = 1;
    Solution sol = getOptimalSol(n, k, points);

    //Double
    while(sol.size() > r){
        k *= 2;
        sol = getOptimalSol(n, k, points);
    }

    //Binary search
    int low = k/2+1, high = k; //k值的候选空间是[low,high], low-1明确不可行
    int fk = k; //于sol相匹配的k值
    while(low < high){
        k = (low + high)/2;
        Solution med_re = getOptimalSol(n, k, points);
        if(med_re.size() <= r){
            high = k;
            sol = med_re;
            fk = k;
        }else low = k + 1;
    }
    cout << "Solutuion: " << sol << " k: "<< fk << "\n";
    RECORD << "Sol size: " << sol.size() << ", fk: "<< fk << "\n";
    
    gettimeofday(&end,NULL);
    unsigned long timer = (end.tv_sec-start.tv_sec)*CONVERTER+(end.tv_usec-start.tv_usec);
    cout << fixed << setprecision(6) << "\ntimer: " << 1.0*timer/CONVERTER << "s\n";
    RECORD << fixed << setprecision(6) << "timer: " << 1.0*timer/CONVERTER << "s\n";

    RECORD << testResultRankR(n, fk, points, sol) << "\n";
    RECORD.close();
    return 0;
}
