#include <fstream>
#include <numeric>
#include <algorithm>
#include <initializer_list>
using namespace std;
int main() { ifstream fin("paint.in");
    ofstream fout("paint.out");
    int a,b,c,d;
    fin >> a >> b >> c >> d;
    if (min(a,b) > max(c,d) | min(c,d) > max(a,b)) {
        fout << (max(a,b)-min(a,b)) + (max(c,d)-min(c,d)) << "\n";
        return 0;}
    fout << max({a,b,c,d})-min({a,b,c,d}) << "\n";}