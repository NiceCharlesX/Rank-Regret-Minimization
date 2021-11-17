#include "2DRRMS/func_io.cpp"
#include "2DRRMS/func_inter.cpp"
#include "2DRRMS/func_basic.cpp"
#include "2DRRMS/func_test.cpp"

Solution getOptimalSol(int const& n, int const& r, Dataset& points, SkylinePointMap& skypointers, RankPointMap& rankpointers){
    IntersectionQ inter_q; //存储交叉点
    vector<vector<Solution>> mat; //动态规划结构, 存储结果集
    PointRankMap pointers; //each point has the pointer from pointid to its rank now

    initializePointers(n, pointers, rankpointers);
    int s = computeSkyline(n, points, skypointers); //skyline size
    initializeIntersectionQueue(n, points, inter_q);
    initializeMatrix(s, r, mat, pointers, skypointers);
    cout << "Skyline Size: " << s << "\n";

    while(!inter_q.empty()){
        Intersection const inter = pop_front_off_queue(inter_q);
        swap(pointers, rankpointers, inter);
        add_intersections(n, inter_q, points, pointers, rankpointers, inter);
        
        SkylineId i_sid = points[inter.i].sid;
        if(i_sid >= 0){// points[inter.i] is a skyline point
            SkylineId j_sid = points[inter.j].sid;
            for(int h = r - 1; h > 0; h--){
                mat[i_sid][h].rankr = max(mat[i_sid][h].rankr, pointers[inter.i] + 1);
                if(j_sid >= 0 && mat[j_sid][h].rankr > mat[i_sid][h-1].rankr){// points[inter.j] is a skyline point, mat[i_sid][h-1].rankr还未更新
                    mat[j_sid][h].rankr = mat[i_sid][h-1].rankr;
                    mat[j_sid][h].sids.assign(mat[i_sid][h-1].sids.begin(), mat[i_sid][h-1].sids.end());
                    mat[j_sid][h].sids.push_back(j_sid);
                }
            }
            mat[i_sid][0].rankr = max(mat[i_sid][0].rankr, pointers[inter.i] + 1);
        }
    }

    SkylineId retid = -1;
    RankR minrank = n;
    for(int i = 0; i < s; i++){
        if(mat[i][r - 1].rankr <= minrank){
            minrank = mat[i][r - 1].rankr;
            retid = i;
        }
	}
    return mat[retid][r-1];
}



int main(int argc, char *argv[])
{
    int n = 10000, r = 5, fid = 3;
    ofstream RECORD("../record/count.txt", ios::app); //app = append
    if(argc > 1) n = stoi(string(argv[1]));
    if(argc > 2) r = stoi(string(argv[2]));
    if(argc > 3) fid = stoi(string(argv[3]));
    cout<<"\n2DRRMS "<<n<<" "<<r<<" "<<fid<<"\n";
    RECORD<<"\n2DRRMS "<<n<<" "<<r<<" "<<fid<<"\n";

    Dataset points; // 所有数据第一次排序(按第二个属性降序)后, 就不会更改在points中的顺序
    SkylinePointMap skypointers; //the j-th skyline point has a skypointer from skylineid (j) to its pointid
    RankPointMap rankpointers; //rank i tuple has a rankpointer to its pointid, i.e., the Inverse pointers of "pointers"
    read_file(points, n, fid);

    struct timeval start, end;
    gettimeofday(&start,NULL);

    sort_points(points);
    Solution sol = getOptimalSol(n, r, points, skypointers, rankpointers);
    cout << "Solution: " << sol << "\n";
    RECORD << "Sol: " << sol << "\n";

    gettimeofday(&end,NULL);
    unsigned long timer = (end.tv_sec-start.tv_sec)*CONVERTER+(end.tv_usec-start.tv_usec);
    cout << fixed << setprecision(6) << "\ntimer: " << 1.0*timer/CONVERTER << "s\n";
    RECORD << fixed << setprecision(6) << "timer: " << 1.0*timer/CONVERTER << "s\n";

    RECORD << testResultRankR(n, points, skypointers, sol) << " " << testRankPointer(n, points, rankpointers) << "\n";
    RECORD.close();
    return 0;
}
