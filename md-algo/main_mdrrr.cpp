#include "MDRRR/func_io.cpp"
#include "MDRRR/func_basic.cpp"
#include "MDRRR/func_enum_md.cpp" //调用了func_topk
#include "MDRRR/sc/func_sc3.cpp"
#include "MDRRR/sc/func_sc_md.cpp"

int main(int argc, char *argv[])
{
    int n = 10000, d = 6, r = 10, fid = 3, constrain_n = 0;
    ofstream RECORD("../record/count.txt", ios::app); //app = append
    if(argc > 1) n = stoi(string(argv[1]));
    if(argc > 2) d = stoi(string(argv[2]));
    if(argc > 3) r = stoi(string(argv[3]));
    if(argc > 4) fid = stoi(string(argv[4]));
    if(argc > 5) constrain_n = stoi(string(argv[5])); //{w_i>=w_{i+1}| i\in{1,...,c}}.
    cout<<"\nMDRRR "<<n<<" "<<d<<" "<<r<<" "<<fid<< " cn: " << constrain_n<< "\n";
    RECORD<<"\nMDRRR "<<n<<" "<<d<<" "<<r<<" "<<fid<< " cn: " << constrain_n<<"\n";
    Dataset points;
    read_file(points, n, d, fid);

    struct timeval start, end;
    gettimeofday(&start,NULL);

    int k = 1;
    Solution sol = SetCoverTheo(KSetEnum(constrain_n, points, n, d, k, TM), n);
    //Double
    while(sol.size() > r){
        k *= 2;
        sol = SetCoverTheo(KSetEnum(constrain_n, points, n, d, k, TM), n);
    }

    //Binary search
    int low = k/2+1, high = k, fk = k; //k值的候选空间是[low,high], low-1明确不可行
    while(low < high){
        k = (low + high)/2;
        Solution med_re = SetCoverTheo(KSetEnum(constrain_n, points, n, d, k, TM), n);
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

    vector<UtilityVector> uvs_v = initialAllUtilityVectors(d, SSV, constrain_n); //utility vectors for verification
    RankR rrr = testRankRegret(points, sol, uvs_v);
    cout << "real rank-regret: " << rrr << "\n";
    RECORD << "rrr: " << rrr << "\n";
    RECORD.close();
    return 0;
}

