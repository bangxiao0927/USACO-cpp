#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

namespace {

constexpr int MAX_BIT = 30;
constexpr int MAX_ROWS = 100;

bool build_counts(long long M, int target_bits, vector<int>& counts) {
    counts.assign(MAX_BIT + 1, 0);
    auto dfs = [&](auto&& self, int pos, long long rem_value, int rem_bits) -> bool {
        if (rem_bits < 0 || rem_value < 0) {
            return false;
        }
        if (pos < 0) {
            return rem_value == 0 && rem_bits == 0;
        }
        long long pow = 1LL << pos;
        int max_cnt = min({MAX_ROWS, rem_bits, static_cast<int>(rem_value / pow)});
        vector<int> order;
        if (max_cnt >= 1) {
            for (int cnt = 1; cnt <= max_cnt; ++cnt) {
                order.push_back(cnt);
            }
            order.push_back(0);
        } else {
            order.push_back(0);
        }
        for (int cnt : order) {
            long long next_value = rem_value - pow * cnt;
            int next_bits = rem_bits - cnt;
            if (pos == 0) {
                if (next_value == 0 && next_bits == 0) {
                    counts[pos] = cnt;
                    return true;
                }
                continue;
            }
            long long min_possible = next_bits;
            long long max_possible = next_bits * (1LL << (pos - 1));
            if (next_value < min_possible || next_value > max_possible) {
                continue;
            }
            counts[pos] = cnt;
            if (self(self, pos - 1, next_value, next_bits)) {
                return true;
            }
        }
        return false;
    };
    return dfs(dfs, MAX_BIT, M, target_bits);
}

bool find_parity_subset(int active_cols, int K, vector<int>& subset_values, int& subset_sum) {
    const int INF = 1e9;
    vector<int> best_sum(32, INF);
    vector<int> best_mask(32, 0);
    best_sum[0] = 0;
    for (int v = 1; v <= active_cols; ++v) {
        vector<int> next_sum = best_sum;
        vector<int> next_mask = best_mask;
        for (int x = 0; x < 32; ++x) {
            if (best_sum[x] == INF) {
                continue;
            }
            int nx = x ^ v;
            int candidate = best_sum[x] + v;
            if (candidate < next_sum[nx]) {
                next_sum[nx] = candidate;
                next_mask[nx] = best_mask[x] | (1 << (v - 1));
            }
        }
        best_sum.swap(next_sum);
        best_mask.swap(next_mask);
    }

    if (best_sum[K] == INF) {
        return false;
    }
    subset_sum = best_sum[K];
    subset_values.clear();
    int mask = best_mask[K];
    for (int v = 1; v <= active_cols; ++v) {
        if (mask & (1 << (v - 1))) {
            subset_values.push_back(v);
        }
    }
    return true;
}

bool assign_bits(const vector<int>& row_pops, const vector<int>& counts, vector<long long>& values) {
    vector<int> col_remaining(MAX_BIT + 1);
    for (int j = 0; j <= MAX_BIT; ++j) {
        col_remaining[j] = counts[j];
    }

    struct Row {
        int need;
        int idx;
        long long mask;
    };

    vector<Row> rows;
    for (int i = 0; i < static_cast<int>(row_pops.size()); ++i) {
        rows.push_back({row_pops[i], i, 0LL});
    }

    vector<int> order(rows.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return rows[a].need > rows[b].need;
    });

    for (int id : order) {
        int need = rows[id].need;
        if (need == 0) {
            continue;
        }
        vector<int> candidates;
        for (int j = 0; j <= MAX_BIT; ++j) {
            if (col_remaining[j] > 0) {
                candidates.push_back(j);
            }
        }
        sort(candidates.begin(), candidates.end(), [&](int x, int y) {
            return col_remaining[x] > col_remaining[y];
        });
        if (static_cast<int>(candidates.size()) < need) {
            return false;
        }
        for (int t = 0; t < need; ++t) {
            int bit = candidates[t];
            rows[id].mask |= (1LL << bit);
            col_remaining[bit]--;
        }
    }

    for (int rem : col_remaining) {
        if (rem != 0) {
            return false;
        }
    }

    values.assign(rows.size(), 0);
    for (const Row& row : rows) {
        values[row.idx] = row.mask;
    }
    return true;
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) {
        return 0;
    }

    while (T--) {
        long long M;
        int K;
        cin >> M >> K;

        const int base_bits = __builtin_popcountll(M);
        const int upper_bits = min<long long>(M, MAX_ROWS * 31LL);
        bool solved = false;
        vector<long long> answer;

        for (int total_bits = base_bits; total_bits <= upper_bits; ++total_bits) {
            vector<int> counts;
            if (!build_counts(M, total_bits, counts)) {
                continue;
            }

            int active_cols = 0;
            int max_column = 0;
            for (int val : counts) {
                if (val > 0) {
                    ++active_cols;
                    max_column = max(max_column, val);
                }
            }
            if (active_cols == 0) {
                continue;
            }

            vector<int> parity_set;
            int parity_sum = 0;
            if (!find_parity_subset(active_cols, K, parity_set, parity_sum)) {
                continue;
            }

            if (parity_sum > total_bits) {
                continue;
            }
            int remaining_bits = total_bits - parity_sum;
            if (remaining_bits % 2 == 1) {
                continue;
            }
            int target_pairs_sum = remaining_bits / 2;

            int parity_count = static_cast<int>(parity_set.size());
            if (parity_count > MAX_ROWS) {
                continue;
            }

            int rows_needed = max_column;
            int low_pairs = max(0, (rows_needed - parity_count + 1) / 2);
            int low_sum_limit = (active_cols == 0) ? 0 : (target_pairs_sum + active_cols - 1) / active_cols;
            low_pairs = max(low_pairs, low_sum_limit);
            int high_pairs = min(target_pairs_sum, (MAX_ROWS - parity_count) / 2);
            if (low_pairs > high_pairs) {
                continue;
            }

            int pairs = low_pairs;
            vector<int> pair_values(pairs, 1);
            int remaining = target_pairs_sum - pairs;
            for (int i = 0; i < pairs && remaining > 0; ++i) {
                int add = min(active_cols - 1, remaining);
                pair_values[i] += add;
                remaining -= add;
            }
            if (remaining != 0) {
                continue;
            }

            vector<int> row_popcounts;
            row_popcounts.reserve(parity_count + 2 * pairs);
            for (int val : parity_set) {
                row_popcounts.push_back(val);
            }
            for (int val : pair_values) {
                row_popcounts.push_back(val);
                row_popcounts.push_back(val);
            }

            if (row_popcounts.empty() || static_cast<int>(row_popcounts.size()) > MAX_ROWS) {
                continue;
            }

            vector<long long> values;
            if (!assign_bits(row_popcounts, counts, values)) {
                continue;
            }

            long long sum_check = 0;
            for (long long v : values) {
                sum_check += v;
            }
            if (sum_check != M) {
                continue;
            }

            solved = true;
            answer = values;
            break;
        }

        if (!solved) {
            cout << -1 << '\n';
        } else {
            cout << answer.size() << '\n';
            for (size_t i = 0; i < answer.size(); ++i) {
                if (i) cout << ' ';
                cout << answer[i];
            }
            cout << '\n';
        }
    }

    return 0;
}
