#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

long long solve_min(long long base, const vector<long long>& delta, int target) {
    const long long INF = (long long)4e18;
    long long neg_sum = 0;
    int neg_cnt = 0;
    bool has_zero = false;
    long long remove_penalty = INF;
    long long add_penalty = INF;

    for (long long d : delta) {
        if (d < 0) {
            neg_sum += d;
            ++neg_cnt;
            remove_penalty = min(remove_penalty, -d);
        } else if (d == 0) {
            has_zero = true;
        } else {
            add_penalty = min(add_penalty, d);
        }
    }

    long long res = base + neg_sum;
    if ((neg_cnt & 1) == target || has_zero) return res;

    long long best = INF;
    if (remove_penalty < INF) best = min(best, res + remove_penalty);
    if (add_penalty < INF) best = min(best, res + add_penalty);
    return best;
}

long long solve_max(long long base, const vector<long long>& delta, int target) {
    const long long INF = (long long)4e18;
    long long pos_sum = 0;
    int pos_cnt = 0;
    bool has_zero = false;
    long long remove_penalty = INF;
    long long add_penalty = INF;

    for (long long d : delta) {
        if (d > 0) {
            pos_sum += d;
            ++pos_cnt;
            remove_penalty = min(remove_penalty, d);
        } else if (d == 0) {
            has_zero = true;
        } else {
            add_penalty = min(add_penalty, -d);
        }
    }

    long long res = base + pos_sum;
    if ((pos_cnt & 1) == target || has_zero) return res;

    long long best = -INF;
    if (remove_penalty < INF) best = max(best, res - remove_penalty);
    if (add_penalty < INF) best = max(best, res - add_penalty);
    return best;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, k;
        string s;
        cin >> n >> k >> s;

        int limit = n - k;
        vector<int> jump(max(limit, 0));
        for (int i = 0; i < limit; ++i) {
            jump[i] = (s[i] - '0') ^ (s[i + 1] - '0');
        }

        vector<long long> keep_penalty(k), flip_penalty(k);
        for (int r = 0; r < k; ++r) {
            long long keep = 0, flip = 0;
            int state = 0;
            for (int pos = r; pos < n; pos += k) {
                if (state == 0)
                    ++flip;
                else
                    ++keep;
                if (pos >= limit) break;
                state ^= jump[pos];
            }
            keep_penalty[r] = keep;
            flip_penalty[r] = flip;
        }

        long long base = 0;
        vector<long long> delta(k);
        for (int r = 0; r < k; ++r) {
            base += keep_penalty[r];
            delta[r] = flip_penalty[r] - keep_penalty[r];
        }

        int target = s[0] - '0';
        cout << solve_min(base, delta, target) << ' ' << solve_max(base, delta, target) << '\n';
    }

    return 0;
}
