#include "MDRC/func_io.cpp"
#include "MDRC/func_basic.cpp"
#include "MDRC/func_imv.cpp"
#include "MDRC/func_mdrc.cpp" //调用了func_imv

int main(int argc, char *argv[])
{
    int n = 10000, d = 3, r = 10, fid = 3;
    ofstream RECORD("../record/count.txt", ios::app); //app = append
    if(argc > 1) n = stoi(string(argv[1]));
    if(argc > 2) d = stoi(string(argv[2]));
    if(argc > 3) r = stoi(string(argv[3]));
    if(argc > 4) fid = stoi(string(argv[4]));
    cout<<"\nMDRC "<<n<<" "<<d<<" "<<r<<" "<<fid<<"\n";
    RECORD<<"\nMDRC "<<n<<" "<<d<<" "<<r<<" "<<fid<<"\n";
    Dataset points;
    read_file(points, n, d, fid);

    struct timeval start, end;
    gettimeofday(&start,NULL);

    //Binary search
    int low = 1, high = n; //k值的候选空间是[low,high], low-1明确不可行
    Solution sol;
    int fk = n; //于sol相匹配的k值
    while(low < high){
        int k = (low + high)/2;
        cout << k << " ";
        if(k == 1) break;
        Solution med_re = initialAndStart(points, n, d, k);
        if(med_re.size() <= r){
            high = k;
            sol = med_re;
            fk = k;
        }else low = k + 1;
    }
    cout << "\n";

    cout << "Solutuion: " << sol << " k: "<< fk << "\n";
    RECORD << "Sol size: " << sol.size() << ", fk: "<< fk << "\n";

    gettimeofday(&end,NULL);
    unsigned long timer = (end.tv_sec-start.tv_sec)*CONVERTER+(end.tv_usec-start.tv_usec);
    cout << fixed << setprecision(6) << "\ntimer: " << 1.0*timer/CONVERTER << "s\n";
    RECORD << fixed << setprecision(6) << "timer: " << 1.0*timer/CONVERTER << "s\n";

    vector<UtilityVector> uvs_v = initialAllUtilityVectors(d, SSV); //utility vectors for verification
    RankR rrr = testRankRegret(points, sol, uvs_v);
    cout << "real rank-regret: " << rrr << "\n";
    RECORD << "rrr: " << rrr << "\n";
    RECORD.close();

    return 0;
}
