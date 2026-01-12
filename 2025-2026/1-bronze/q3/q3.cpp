#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N, K;
    if (!(cin >> N >> K)) {
        return 0;
    }
    int Q;
    cin >> Q;

    const int limit = N - K + 1;
    vector<vector<long long>> grid(N + 1, vector<long long>(N + 1, 0));
    vector<vector<long long>> window(limit + 1, vector<long long>(limit + 1, 0));
    long long best = 0;

    while (Q--) {
        int r, c;
        long long v;
        cin >> r >> c >> v;
        long long delta = v - grid[r][c];
        grid[r][c] = v;

        int row_start = max(1, r - K + 1);
        int row_end = min(r, limit);
        int col_start = max(1, c - K + 1);
        int col_end = min(c, limit);

        for (int i = row_start; i <= row_end; ++i) {
            for (int j = col_start; j <= col_end; ++j) {
                window[i][j] += delta;
                best = max(best, window[i][j]);
            }
        }

        cout << best << '\n';
    }

    return 0;
}
