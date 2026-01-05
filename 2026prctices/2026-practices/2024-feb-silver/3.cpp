// Solution for Bessie and Elsie marble parity guessing game.
// We precompute, for each round, how Elsie's guess affects her marble count
// under Bessie's optimal (worst-case) response, then find the lexicographically
// smallest sequence of guesses that keeps her marble count positive.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        long long N;
        int M, K;
        cin >> N >> M >> K;
        vector<long long> d_even(M + 1), d_odd(M + 1);
        for (int i = 1; i <= M; ++i) {
            long long min_even = (1LL << 60), max_even = -1;
            long long min_odd = (1LL << 60), max_odd = -1;
            bool has_even = false, has_odd = false;
            for (int j = 0; j < K; ++j) {
                int x;
                cin >> x;
                if (x % 2 == 0) {
                    has_even = true;
                    min_even = min(min_even, (long long)x);
                    max_even = max(max_even, (long long)x);
                } else {
                    has_odd = true;
                    min_odd = min(min_odd, (long long)x);
                    max_odd = max(max_odd, (long long)x);
                }
            }

            if (has_odd) {
                d_even[i] = -max_odd;
            } else {
                d_even[i] = min_even;  // all numbers even, so guessing even forces gain
            }

            if (has_even) {
                d_odd[i] = -max_even;
            } else {
                d_odd[i] = min_odd;  // all numbers odd, so guessing odd forces gain
            }
        }

        vector<long long> need(M + 2, 1);
        need[M + 1] = 1;  // after last round we just need to stay positive
        for (int i = M; i >= 1; --i) {
            long long req_even = max(1LL, need[i + 1] - d_even[i]);
            long long req_odd = max(1LL, need[i + 1] - d_odd[i]);
            need[i] = min(req_even, req_odd);
        }

        if (N < need[1]) {
            cout << -1 << '\n';
            continue;
        }

        vector<string> answer;
        answer.reserve(M);
        long long cur = N;
        for (int i = 1; i <= M; ++i) {
            if (cur + d_even[i] >= need[i + 1]) {
                answer.push_back("Even");
                cur += d_even[i];
            } else {
                answer.push_back("Odd");
                cur += d_odd[i];
            }
        }

        for (int i = 0; i < M; ++i) {
            if (i) cout << ' ';
            cout << answer[i];
        }
        cout << '\n';
    }

    return 0;
}
