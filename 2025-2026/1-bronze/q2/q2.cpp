#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
using namespace std;

int main() {
    int T, k;
    if (!(cin >> T >> k)) return 0;
    while (T--) {
        int FjString;
        cin >> FjString;
        string S;
        cin >> S;
        int N = FjString;
        int total = 3 * N;
        if (N % 2 == 1) {
            cout << -1 << '\n';
            continue;
        }
        int halfBlocks = N / 2;
        int prefixLen = 3 * halfBlocks;
        bool wholeSquare = true;
        for (int i = 0; i < prefixLen; ++i) {
            if (S[i] != S[i + prefixLen]) {
                wholeSquare = false;
                break;
            }
        }
        if (wholeSquare) {
            cout << 1 << '\n';
            for (int i = 0; i < total; ++i) {
                cout << 1 << (i + 1 == total ? '\n' : ' ');
            }
            continue;
        }
        vector<vector<int>> pref(3), suff(3), allpos(3);
        for (int i = 0; i < total; ++i) {
            int id = S[i] == 'C' ? 0 : (S[i] == 'O' ? 1 : 2);
            allpos[id].push_back(i);
            if (i < prefixLen) pref[id].push_back(i);
            else suff[id].push_back(i - prefixLen);
        }
        vector<int> match(prefixLen);
        for (int id = 0; id < 3; ++id) {
            for (int j = 0; j < (int)pref[id].size(); ++j) {
                match[pref[id][j]] = suff[id][j];
            }
        }
        vector<int> assignPrefix(prefixLen);
        vector<int> last = {-1, -1};
        array<int, 2> order = {0, 1};
        bool ok = true;
        for (int i = 0; i < prefixLen; ++i) {
            int v = match[i];
            sort(order.begin(), order.end(), [&](int a, int b) {
                return last[a] > last[b];
            });
            bool placed = false;
            for (int idx = 0; idx < 2; ++idx) {
                int id = order[idx];
                if (v > last[id]) {
                    assignPrefix[i] = id + 1;
                    last[id] = v;
                    placed = true;
                    break;
                }
            }
            if (!placed) {
                ok = false;
                break;
            }
        }
        if (!ok) {
            cout << 3 << '\n';
            vector<int> ans(total);
            for (int id = 0; id < 3; ++id) {
                int op = id + 1;
                for (int pos : allpos[id]) ans[pos] = op;
            }
            for (int i = 0; i < total; ++i) cout << ans[i] << (i + 1 == total ? '\n' : ' ');
            continue;
        }
        vector<int> assignSuffix(prefixLen);
        for (int id = 0; id < 3; ++id) {
            for (int j = 0; j < (int)pref[id].size(); ++j) {
                int p = pref[id][j];
                int q = suff[id][j];
                assignSuffix[q] = assignPrefix[p];
            }
        }
        cout << 2 << '\n';
        vector<int> ans(total);
        for (int i = 0; i < prefixLen; ++i) ans[i] = assignPrefix[i];
        for (int i = 0; i < prefixLen; ++i) ans[prefixLen + i] = assignSuffix[i];
        for (int i = 0; i < total; ++i) cout << ans[i] << (i + 1 == total ? '\n' : ' ');
    }
    return 0;
}
