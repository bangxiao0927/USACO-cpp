#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>
#include <cmath>
using namespace std;

void solve(){
    ifstream fin("traffic.in");
    ofstream fout("traffic.out");

    int n;
    fin >> n;

    string type[100];
    int low[100];
    int high[100];

    for (int i = 0; i < n; i++){
        string t;
        int l, h;
        fin >> t >> l >> h;
        type[i] = t;
        low[i] = l;
        high[i] = h;
    }

    int ans_lower = -10000, ans_higher = 10000;

    for (int i = n - 1; i >= 0; i--){
        if (type[i] == "none"){
            ans_lower = max(ans_lower, low[i]);
            ans_higher = min(ans_higher, high[i]);
        }
        else if (type[i] == "off"){
            ans_lower += low[i];
            ans_higher += high[i];
        }
        else if (type[i] == "on"){
            ans_higher -= low[i];
            ans_lower -= high[i];
        }
    }

    fout << max(0, ans_lower) << ' ' << ans_higher << '\n';

    ans_lower = -10000;
    ans_higher = 10000;

    for (int i = 0; i < n; i++){
        if (type[i] == "none"){
            ans_lower = max(ans_lower, low[i]);
            ans_higher = min(ans_higher, high[i]);
        }
        else if (type[i] == "on"){
            ans_lower += low[i];
            ans_higher += high[i];
        }
        else if (type[i] == "off"){
            ans_higher -= low[i];
            ans_lower -= high[i];
        }
    }

    fout << max(0, ans_lower) << ' ' << ans_higher << '\n';

}

int main() {
    solve();

    return 0;
}