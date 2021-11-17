#include "HDRRMS/func_io.cpp"
#include "HDRRMS/func_basic.cpp"
#include "HDRRMS/func_basis.cpp"
#include "HDRRMS/func_enum_nth.cpp" //基于nth, 调用了func_basic
#include "HDRRMS/func_sc.cpp"
#include "HDRRMS/func_sort.cpp"

int main(int argc, char *argv[])
{
    int n = 10000, d = 4, r = 10, fid = 3, gamma = 6, constrain_n = 0;
    float delta = 0.03;
    ofstream RECORD("../record/count.txt", ios::app); //app = append
    if(argc > 1) n = stoi(string(argv[1]));
    if(argc > 2) d = stoi(string(argv[2]));
    if(argc > 3) r = stoi(string(argv[3]));
    if(argc > 4) fid = stoi(string(argv[4]));
    if(argc > 5) delta = stof(string(argv[5]));
    if(argc > 6) gamma = stoi(string(argv[6]));
    if(argc > 7) constrain_n = stoi(string(argv[7])); //{w_i>=w_{i+1}| i\in{1,...,c}}.

    int ssh = 32768, sd = 343; //ssh: sample size for HDRRMS, sd: size for discretization
    if(delta == 0) ssh = 0;
    else ssh = (int)((r-d)*log(n-d)+log(n-r+1)+log(n))/(2*pow(delta-1.0/n,2));
    if(gamma == 0) sd = 0;
    else sd = pow(gamma+1,d-1);

    cout<<"\nHDRRMS "<<n<<" "<<d<<" "<<r<<" "<<fid<<" "<<delta<< " " << gamma<< " size:" <<ssh << " " << sd <<" cn:" << constrain_n << "\n";
    RECORD<<"\nHDRRMS "<<n<<" "<<d<<" "<<r<<" "<<fid<<" "<<delta<<" "<< gamma<< " size:" <<ssh << " " << sd <<" cn:" << constrain_n << "\n";
    Dataset points;
    read_file(points, n, d, fid);

    struct timeval start, end0, end1;
    gettimeofday(&start,NULL);

    sort_points(points);
    Solution basis = getBasisAndFilter(points, n, d);
    cout << "size after filtering: " << n << ", Basis: " << basis << "\n";

    int k = 1;
    
    vector<UtilityVector> uvs = initialAllUtilityVectors(d, ssh, gamma, constrain_n);
    vector<vector<UItem>> uisl(uvs.size());
    cout << "uvs.size(): " << uvs.size() << "\n";
    Solution sol = SetCover(initialUItemsLists(uisl, uvs, points, n, basis), n);

    //Double
    while(sol.size() > r - basis.size()){
        k *= 2;
        sol = SetCover(IncreKSetEnum(uisl, k), n);
    }

    //Binary search
    int low = k/2+1, high = k; //k值的候选空间是[low,high], low-1明确不可行
    int fk = k; //于sol相匹配的k值
    while(low < high){
        k = (low + high)/2;
        Solution med_re = SetCover(DecreKSetEnum(uisl, k, low, high), n); 
        if(med_re.size() + basis.size() <= r){
            high = k;
            sol = med_re;
            fk = k;
        }else low = k + 1;
    }

    sol.insert(basis.begin(), basis.end());
    gettimeofday(&end0,NULL);

    cout << "Sol size before: " << sol.size() <<", ";
    supplementSolution(constrain_n, points, sol, n, d, r, fk, ssh);
    cout << "Solutuion: " << sol << " k: "<< fk << "\n";
    RECORD << "n saf: " << n << ", Basis size: " << basis.size() << ", Sol size: " << sol.size() << ", fk: "<< fk << "\n";

    gettimeofday(&end1,NULL);
    unsigned long timer0 = (end0.tv_sec-start.tv_sec)*CONVERTER+(end0.tv_usec-start.tv_usec), timer1 = (end1.tv_sec-start.tv_sec)*CONVERTER+(end1.tv_usec-start.tv_usec);
    cout << fixed << setprecision(6) << "\ntimer0: " << 1.0*timer0/CONVERTER << "s, timer1: " << 1.0*timer1/CONVERTER << "s\n";
    RECORD << fixed << setprecision(6) << "timer: " << 1.0*timer0/CONVERTER << "s, " << 1.0*timer1/CONVERTER << "s\n";

    vector<UtilityVector> uvs_v = initialAllUtilityVectors(d, SSV, 0, constrain_n); //utility vectors for verification
    RankR rrr = testRankRegret(points, sol, uvs_v);
    RankR prr = testRankRegret(points, sol, uvs);
    cout << "predict rank-regret: " << prr << "\nreal rank-regret: " << rrr << "\n";
    RECORD << "fk: " << prr << ", rrr: " << rrr << "\n";
    RECORD.close();
    return 0;
}

