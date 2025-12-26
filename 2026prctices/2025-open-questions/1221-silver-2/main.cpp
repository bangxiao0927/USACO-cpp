#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

using ll = long long;

// 模拟在环上额外进行 k 次“自配对”（相当于切掉 k 条虚拟边），然后把环变成链再贪心
// 返回能配对的总对数，如果不合法返回极小值
long long simulateCycle(const vector<ll>& baseCaps, ll edgeCount, vector<ll>* leftover) {
    static constexpr ll kNegInf = numeric_limits<ll>::min() / 4;
    if (edgeCount < 0) return kNegInf;

    vector<ll> current = baseCaps;
    
    // 环上额外自配对的数量不能超过首尾的容量（实际上环上任意一点都行）
    if (edgeCount > current.front() || edgeCount > current.back()) {
        return kNegInf;
    }

    // 模拟切掉 edgeCount 对自配对 → 首尾各减去 edgeCount（相当于断开环）
    current.front() -= edgeCount;
    current.back() -= edgeCount;
    ll total = edgeCount;  // 已经配好的自配对数量

    // 现在当成一条链，依次贪心取相邻两点的 min
    for (size_t i = 0; i + 1 < current.size(); ++i) {
        ll take = min(current[i], current[i + 1]);
        current[i] -= take;
        current[i + 1] -= take;
        total += take;
    }

    // 如果需要，返回剩余的奶牛数量
    if (leftover != nullptr) {
        *leftover = std::move(current);
    }
    return total;
}

// 处理一个环形连通分量
// 返回这个环能贡献的最大配对数，并更新剩余奶牛数量
long long solveCycle(const vector<int>& order, vector<ll>& remaining) {
    vector<ll> baseCaps;
    baseCaps.reserve(order.size());
    for (int idx : order) {
        baseCaps.push_back(remaining[idx]);
    }

    // 环上最多能额外自配对的次数受限于最小点权（实际上首尾就够了）
    ll limit = min(baseCaps.front(), baseCaps.back());
    ll low = 0, high = limit;

    // 用三分搜索快速逼近最优解（因为函数在一定范围内先增后减）
    while (high - low > 3) {
        ll mid1 = low + (high - low) / 3;
        ll mid2 = high - (high - low) / 3;
        ll val1 = simulateCycle(baseCaps, mid1, nullptr);
        ll val2 = simulateCycle(baseCaps, mid2, nullptr);
        if (val1 < val2) {
            low = mid1 + 1;
        } else {
            high = mid2 - 1;
        }
    }

    // 小范围暴力找真正的最优
    ll bestPairs = -1;
    ll bestChoice = low;
    for (ll pick = low; pick <= high; ++pick) {
        ll value = simulateCycle(baseCaps, pick, nullptr);
        if (value > bestPairs) {
            bestPairs = value;
            bestChoice = pick;
        }
    }

    // 最后用最优的方案更新剩余奶牛数量
    vector<ll> leftover;
    ll gained = simulateCycle(baseCaps, bestChoice, &leftover);
    for (size_t i = 0; i < order.size(); ++i) {
        remaining[order[i]] = leftover[i];
    }
    return gained;
}

// 处理一条路径（链）上的最大配对
long long solvePath(const vector<int>& order, vector<ll>& remaining) {
    ll gained = 0;
    // 直接沿着链每条边贪心取 min
    for (size_t i = 0; i + 1 < order.size(); ++i) {
        int u = order[i];
        int v = order[i + 1];
        ll take = min(remaining[u], remaining[v]);
        remaining[u] -= take;
        remaining[v] -= take;
        gained += take;
    }
    return gained;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;           // 唯一 ID 数量
    ll A, B;         // 两个目标和
    cin >> N >> A >> B;

    vector<ll> counts(N);               // 每个 ID 的奶牛数量
    vector<ll> values(N);               // 每个 ID 的值
    unordered_map<ll, int> indexOf;     // ID 值 -> 数组下标
    indexOf.reserve(static_cast<size_t>(N) * 2);
    indexOf.max_load_factor(0.7);

    // 读入数据
    for (int i = 0; i < N; ++i) {
        ll herdSize, code;
        cin >> herdSize >> code;
        counts[i] = herdSize;
        values[i] = code;
        indexOf[code] = i;
    }

    // 建图：邻接表
    vector<vector<int>> adjacency(N);
    vector<char> canSelfPair(N, false);  // 是否能自配对（2*d == A 或 2*d == B）

    for (int i = 0; i < N; ++i) {
        ll value = values[i];
        // 判断是否可以自配对
        if (value * 2 == A || value * 2 == B) {
            canSelfPair[i] = true;
        }

        // 尝试连接到 A - value 和 B - value
        auto tryConnect = [&](ll target) {
            auto it = indexOf.find(target);
            if (it == indexOf.end()) return;
            int partner = it->second;
            if (partner == i) return;  // 不能连自己（自配对另算）
            // 避免重复加边
            if (find(adjacency[i].begin(), adjacency[i].end(), partner) != adjacency[i].end()) return;
            adjacency[i].push_back(partner);
            adjacency[partner].push_back(i);
        };

        tryConnect(A - value);
        tryConnect(B - value);
    }

    vector<ll> remaining = counts;  // 当前剩余奶牛数量
    vector<char> visited(N, false);
    ll answer = 0;

    vector<int> stack, component;
    stack.reserve(N);
    component.reserve(N);

    // 遍历所有连通分量
    for (int i = 0; i < N; ++i) {
        if (visited[i]) continue;

        // DFS 找连通分量
        stack.push_back(i);
        visited[i] = true;
        component.clear();

        while (!stack.empty()) {
            int node = stack.back(); stack.pop_back();
            component.push_back(node);
            for (int neighbor : adjacency[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    stack.push_back(neighbor);
                }
            }
        }

        // 如果这个分量没有任何边，直接跳过（孤立点留到最后自配对）
        bool hasEdge = false;
        for (int node : component) {
            if (!adjacency[node].empty()) {
                hasEdge = true;
                break;
            }
        }
        if (!hasEdge) continue;

        // 判断是环还是路径
        vector<int> endpoints;
        bool isCycle = true;
        for (int node : component) {
            size_t deg = adjacency[node].size();
            if (deg != 2) isCycle = false;
            if (deg == 1) endpoints.push_back(node);
        }

        // 处理路径（有端点）
        if (!isCycle) {
            int start = endpoints.empty() ? component.front() : endpoints.front();
            vector<int> order;
            order.reserve(component.size());

            // 从某个端点开始遍历整条链
            int previous = -1;
            int current = start;
            while (true) {
                order.push_back(current);
                int next = -1;
                for (int neighbor : adjacency[current]) {
                    if (neighbor != previous) {
                        next = neighbor;
                        break;
                    }
                }
                if (next == -1) break;
                previous = current;
                current = next;
                if (current == start) break;  // 意外成环（理论上不会）
            }
            answer += solvePath(order, remaining);
        } 
        // 处理环
        else {
            vector<int> order;
            order.reserve(component.size());

            // 随便选一个起点，完整走一圈
            int start = component.front();
            int previous = -1;
            int current = start;
            do {
                order.push_back(current);
                int next = -1;
                for (int neighbor : adjacency[current]) {
                    if (neighbor != previous) {
                        next = neighbor;
                        break;
                    }
                }
                previous = current;
                current = next;
            } while (current != start);

            answer += solveCycle(order, remaining);
        }
    }

    // 最后统一处理所有能自配对的组（包括孤立点）
    for (int i = 0; i < N; ++i) {
        if (canSelfPair[i]) {
            ll extra = remaining[i] / 2;
            answer += extra;
            remaining[i] -= extra * 2;
        }
    }

    cout << answer << '\n';
    return 0;
}