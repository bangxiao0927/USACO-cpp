// USACO 2024 US Open Contest, Silver
// Problem 3. The 'Winning' Gene

#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

struct LCPHelper {
    const string &s;
    vector<int> sa;
    vector<int> rank;
    vector<int> lcp;
    vector<vector<int>> st;
    vector<int> lg;

    explicit LCPHelper(const string &str) : s(str) {
        build_suffix_array();
        build_lcp();
        build_rmq();
    }

    void build_suffix_array() {
        int n = s.size();
        sa.resize(n);
        rank.resize(n);
        vector<int> tmp(n);
        for (int i = 0; i < n; ++i) {
            sa[i] = i;
            rank[i] = s[i];
        }
        for (int k = 1; k < n; k <<= 1) {
            auto cmp = [&](int a, int b) {
                if (rank[a] != rank[b]) return rank[a] < rank[b];
                int ra = (a + k < n) ? rank[a + k] : -1;
                int rb = (b + k < n) ? rank[b + k] : -1;
                return ra < rb;
            };
            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; ++i) {
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) || cmp(sa[i], sa[i - 1]));
            }
            for (int i = 0; i < n; ++i) {
                rank[i] = tmp[i];
            }
            if (rank[sa[n - 1]] == n - 1) break;
        }
    }

    void build_lcp() {
        int n = s.size();
        lcp.assign(max(0, n - 1), 0);
        int k = 0;
        for (int i = 0; i < n; ++i) {
            int r = rank[i];
            if (r == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[r + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
            if (r < n - 1) lcp[r] = k;
            if (k) --k;
        }
    }

    void build_rmq() {
        int n = s.size();
        int m = max(1, n);
        lg.assign(m + 1, 0);
        for (int i = 2; i <= m; ++i) lg[i] = lg[i / 2] + 1;
        if (lcp.empty()) return;
        int len = lcp.size();
        int max_k = lg[len] + 1;
        st.assign(max_k, vector<int>(len));
        st[0] = lcp;
        for (int k = 1; (1 << k) <= len; ++k) {
            for (int i = 0; i + (1 << k) <= len; ++i) {
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    int range_min(int l, int r) const {
        if (l > r) return INT_MAX;
        int len = r - l + 1;
        int k = lg[len];
        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }

    int get_lcp(int i, int j) const {
        if (i == j) return s.size() - i;
        int ri = rank[i];
        int rj = rank[j];
        if (ri > rj) swap(ri, rj);
        if (lcp.empty()) return 0;
        return range_min(ri, rj - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    string S;
    if (!(cin >> N)) return 0;
    cin >> S;

    LCPHelper helper(S);
    vector<long long> answer(N + 1, 0);

    for (int L = 1; L <= N; ++L) {
        int M = N - L + 1;
        vector<int> idx(M);
        iota(idx.begin(), idx.end(), 0);
        auto cmp = [&](int a, int b) {
            int real_a = a;
            int real_b = b;
            int l = helper.get_lcp(real_a, real_b);
            if (l >= L) return real_a < real_b;
            return S[real_a + l] < S[real_b + l];
        };
        sort(idx.begin(), idx.end(), cmp);
        vector<int> ranks(M);
        for (int order = 0; order < M; ++order) {
            ranks[idx[order]] = order;
        }

        vector<int> prev_less(M), next_less(M);
        vector<int> st;
        st.reserve(M);
        for (int i = 0; i < M; ++i) {
            while (!st.empty() && ranks[st.back()] > ranks[i]) st.pop_back();
            prev_less[i] = st.empty() ? -1 : st.back();
            st.push_back(i);
        }
        st.clear();
        for (int i = M - 1; i >= 0; --i) {
            while (!st.empty() && ranks[st.back()] > ranks[i]) st.pop_back();
            next_less[i] = st.empty() ? M : st.back();
            st.push_back(i);
        }

        vector<int> diff(M + 2, 0);
        for (int i = 0; i < M; ++i) {
            int left_span = i - prev_less[i] - 1;
            int right_span = next_less[i] - i - 1;
            int span = left_span + right_span + 1;
            ++diff[1];
            --diff[span + 1];
        }

        int curr = 0;
        for (int w = 1; w <= M; ++w) {
            curr += diff[w];
            int Psize = curr;
            if (Psize <= 0) continue;
            answer[Psize] += 1;
        }
    }

    for (int v = 1; v <= N; ++v) {
        cout << answer[v] << '\n';
    }

    return 0;
}
