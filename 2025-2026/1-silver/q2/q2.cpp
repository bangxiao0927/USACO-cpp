#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    int to;
    long long sum; // ax + ay = sum
};

struct ComponentInfo {
    bool bad = false;
    bool fixed = false;
    long long forced_t = 0;
    vector<int> nodes;
};

ComponentInfo explore_component(int start,
                                const vector<vector<Edge>> &graph,
                                vector<int> &coef,
                                vector<long long> &shift,
                                vector<char> &seen) {
    queue<int> bfs;
    ComponentInfo info;

    seen[start] = true;
    coef[start] = 1;
    shift[start] = 0;
    bfs.push(start);
    info.nodes.push_back(start);

    while (!bfs.empty() && !info.bad) {
        int u = bfs.front();
        bfs.pop();
        for (const Edge &e : graph[u]) {
            int v = e.to;
            long long need = e.sum;
            if (!seen[v]) {
                seen[v] = true;
                coef[v] = -coef[u];
                shift[v] = need - shift[u];
                bfs.push(v);
                info.nodes.push_back(v);
                continue;
            }

            long long coef_sum = coef[u] + coef[v];
            long long shift_sum = shift[u] + shift[v];
            if (coef_sum == 0) {
                if (shift_sum != need) {
                    info.bad = true;
                    break;
                }
            } else {
                long long num = need - shift_sum;
                if (num % coef_sum != 0) {
                    info.bad = true;
                    break;
                }
                long long t_val = num / coef_sum;
                if (info.fixed && t_val != info.forced_t) {
                    info.bad = true;
                    break;
                }
                info.fixed = true;
                info.forced_t = t_val;
            }
        }
    }

    return info;
}

long long count_fixed_nodes(const ComponentInfo &info,
                           const vector<int> &coef,
                           const vector<long long> &shift,
                           const vector<long long> &L,
                           const vector<long long> &R) {
    long long active = 0;
    for (int node : info.nodes) {
        long long val = coef[node] * info.forced_t + shift[node];
        if (L[node] <= val && val <= R[node]) ++active;
    }
    return active;
}

long long maximize_free_nodes(const ComponentInfo &info,
                              const vector<int> &coef,
                              const vector<long long> &shift,
                              const vector<long long> &L,
                              const vector<long long> &R) {
    const long long INF = numeric_limits<long long>::max() / 4;
    vector<pair<long long, int>> events;
    events.reserve(info.nodes.size() * 2);

    for (int node : info.nodes) {
        long long left_bound, right_bound;
        if (coef[node] == 1) {
            left_bound = L[node] - shift[node];
            right_bound = R[node] - shift[node];
        } else {
            left_bound = shift[node] - R[node];
            right_bound = shift[node] - L[node];
        }
        if (left_bound > right_bound) continue;
        events.push_back({left_bound, +1});
        events.push_back({min(right_bound + 1, INF), -1});
    }

    sort(events.begin(), events.end());
    long long cur = 0, best = 0;
    for (const auto &e : events) {
        cur += e.second;
        best = max(best, cur);
    }
    return best;
}

long long solve_case(int N, const vector<long long> &L, const vector<long long> &R,
                     const vector<vector<Edge>> &graph) {
    vector<int> coef(N + 1, 0);
    vector<long long> shift(N + 1, 0);
    vector<char> seen(N + 1, false);

    long long total_active = 0;
    for (int start = 1; start <= N; ++start) {
        if (seen[start]) continue;
        ComponentInfo info = explore_component(start, graph, coef, shift, seen);
        if (info.bad) return -1;
        total_active += info.fixed ? count_fixed_nodes(info, coef, shift, L, R)
                                   : maximize_free_nodes(info, coef, shift, L, R);
    }

    return total_active;
}

int main() {

    int T;
    cin >> T;
    while (T--) {
        int N, M;
        cin >> N >> M;
        vector<long long> L(N + 1), R(N + 1);
        for (int i = 1; i <= N; ++i) cin >> L[i];
        for (int i = 1; i <= N; ++i) cin >> R[i];

        vector<vector<Edge>> graph(N + 1);
        for (int i = 0; i < M; ++i) {
            int x, y;
            long long z;
            cin >> x >> y >> z;
            graph[x].push_back({y, z});
            graph[y].push_back({x, z});
        }

        cout << solve_case(N, L, R, graph) << '\n';
    }

    return 0;
}
