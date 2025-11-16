#include <fstream>
#include <algorithm>
#include <numeric>
#include <vector>
using namespace std;
int main() {
    ifstream fin("cbarn.in");
    ofstream fout("cbarn.out");
    int n;
    fin >> n;
    vector<int> cow_number;
    cow_number.reserve(n);
    for (int i = 0;i < n;i++) {
        int a;
        fin >> a;
        cow_number.push_back(a);
    }
    int minSteps = 0;
    for (int i =0;i < n;i++) {
        int lingshi = 0;
        for (int d = i+1;d < n+i;d++) {
            int temp = d;
            if (temp>=n) {
                temp -= n;
            }
            lingshi += cow_number[temp] * (d-i);
        }
        if (minSteps == 0 || lingshi < minSteps) {
            minSteps = lingshi;
        }
    }
    fout << minSteps << endl;
}