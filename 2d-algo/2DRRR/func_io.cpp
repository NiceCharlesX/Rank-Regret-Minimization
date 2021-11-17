#include "stru_basic.hpp"

inline
ostream & operator << (ostream & stream, Point const& p){
    return stream << "<" << p[0] << ", " << p[1] << ">";}
inline
ostream & operator << (ostream & stream, Intersection const& inter){
    return stream << "(" << inter.angle << ", <" << inter.i << ", " << inter.j << ">)";}
ostream & operator << (ostream & stream, Solution const& sol){
    string sids;
    for (PointId pid: sol){
        sids.append(to_string(pid));
        sids.append(", ");
	}
    return stream << "(" << sids << "size:" << sol.size() << ")";
}

void read_file(Dataset & points, int const& n, int const& fid){
    char* fileName;
    if(fid == 0) fileName = (char*)"../real/weather_4d_178080n.txt"; 
    else if(fid == 1) fileName = (char*)"../real/NBA-5d-21961n.txt";
    else if(fid == 2) fileName = (char*)"../real/island.txt";
    else if(fid == 3) fileName = (char*)"../real/independent.txt";
    else if(fid == 4) fileName = (char*)"../real/correlated1.txt";
    else fileName = (char*)"../real/anticorrelated1.txt";

	FILE* c_fp;
	if((c_fp = fopen(fileName, "r")) == NULL){
		cout<< "Cannot open the data file %s.\n";
		exit(0);
	}

    int nmax,dmax;
    fscanf(c_fp, "%d", &nmax);
    fscanf(c_fp, "%d", &dmax);
    cout << "nmax: " << nmax << ", dmax: " << dmax <<"\n";

    if(nmax < n || dmax < 2){
		cout<< "Input Parameter Error\n";
		exit(0);
	}

	for(PointId i = 0; i < n; i++){
        Point point;
        float temp;
        for(int j = 0; j < dmax; j++){
            fscanf(c_fp, "%f", &temp);
            if(j < 2) point[j] = 1000000 * temp;
        }
        points.push_back(point);
	}
	fclose(c_fp);
}
