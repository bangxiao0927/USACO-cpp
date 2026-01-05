#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

using int64 = long long;
const int64 INF64 = static_cast<int64>(4e18);
const int64 NEG_INF64 = -INF64;

struct Vertex {
    int64 x;
    int64 y;
    int sign; // 0:any, 1:positive, -1:negative
};

struct Interval {
    int64 low;
    int64 high;
};

int64 floor_div(int64 num, int64 den) {
    if (den < 0) {
        num = -num;
        den = -den;
    }
    if (num >= 0) return num / den;
    return -(( -num + den - 1) / den);
}

int64 ceil_div(int64 num, int64 den) {
    if (den < 0) {
        num = -num;
        den = -den;
    }
    if (num >= 0) return (num + den - 1) / den;
    return -((-num) / den);
}

struct Matcher {
    const std::vector<int64>& slopes;
    std::vector<Vertex> vertices;
    std::vector<Interval> intervals;
    std::vector<int> order;

    Matcher(const std::vector<int64>& s, std::vector<Vertex> v)
        : slopes(s), vertices(std::move(v)), intervals(vertices.size()), order(vertices.size()) {}

    bool can_with_bounds() {
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            if (intervals[a].low != intervals[b].low)
                return intervals[a].low < intervals[b].low;
            return intervals[a].high < intervals[b].high;
        });
        std::priority_queue<int64, std::vector<int64>, std::greater<int64>> pq;
        size_t ptr = 0;
        for (int64 s : slopes) {
            while (ptr < order.size() && intervals[order[ptr]].low <= s) {
                pq.push(intervals[order[ptr]].high);
                ++ptr;
            }
            while (!pq.empty() && pq.top() < s) pq.pop();
            if (pq.empty()) return false;
            pq.pop();
        }
        return true;
    }

    bool can_at_least(int64 L) {
        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto& v = vertices[i];
            int64 high = floor_div(v.y - L, v.x);
            high = std::min(high, INF64);
            int64 low = NEG_INF64;
            if (v.sign == 1) low = std::max<int64>(low, 1);
            if (v.sign == -1) high = std::min<int64>(high, -1);
            if (low > high) return false;
            intervals[i] = {low, high};
        }
        return can_with_bounds();
    }

    bool can_at_most(int64 U) {
        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto& v = vertices[i];
            int64 low = ceil_div(v.y - U, v.x);
            low = std::max(low, NEG_INF64);
            int64 high = INF64;
            if (v.sign == 1) low = std::max<int64>(low, 1);
            if (v.sign == -1) {
                high = -1;
                if (low > high) return false;
            }
            intervals[i] = {low, high};
        }
        return can_with_bounds();
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    if (!(std::cin >> T)) return 0;
    while (T--) {
        int N;
        int64 X1;
        std::cin >> N >> X1;
        std::vector<int64> y1(N), y2(N), x2(N);
        for (int i = 0; i < N; ++i) {
            std::cin >> y1[i] >> y2[i] >> x2[i];
        }
        std::vector<int64> slopes(4 * N);
        for (int i = 0; i < 4 * N; ++i) std::cin >> slopes[i];
        int pos = 0, neg = 0;
        for (int64 s : slopes) {
            if (s > 0) ++pos;
            else if (s < 0) ++neg;
        }
        if (pos < N || neg < N) {
            std::cout << -1 << '\n';
            continue;
        }
        std::sort(slopes.begin(), slopes.end());
        std::vector<Vertex> verts;
        verts.reserve(4 * N);
        for (int i = 0; i < N; ++i) {
            verts.push_back({X1, y1[i], 0});
            verts.push_back({X1, y2[i], 0});
            verts.push_back({x2[i], y1[i], 1});
            verts.push_back({x2[i], y2[i], -1});
        }
        int64 global_min = slopes.front();
        int64 global_max = slopes.back();
        int64 min_pos = INF64, max_pos = -INF64;
        int64 min_neg = INF64, max_neg = -INF64;
        for (int64 s : slopes) {
            if (s > 0) {
                min_pos = std::min(min_pos, s);
                max_pos = std::max(max_pos, s);
            } else if (s < 0) {
                min_neg = std::min(min_neg, s);
                max_neg = std::max(max_neg, s);
            }
        }
        Matcher matcher(slopes, verts);
        int64 minBound = INF64;
        int64 maxBound = -INF64;
        for (const auto& v : matcher.vertices) {
            int64 bestHigh = (v.sign == 1) ? max_pos : (v.sign == -1 ? max_neg : global_max);
            int64 bestLow = (v.sign == 1) ? min_pos : (v.sign == -1 ? min_neg : global_min);
            int64 lowCandidate = static_cast<int64>((__int128)v.y - (__int128)bestHigh * v.x);
            int64 highCandidate = static_cast<int64>((__int128)v.y - (__int128)bestLow * v.x);
            minBound = std::min(minBound, lowCandidate);
            maxBound = std::max(maxBound, highCandidate);
        }
        int64 lo = minBound, hi = maxBound, bestLow = minBound;
        while (lo <= hi) {
            int64 mid = lo + (hi - lo) / 2;
            if (matcher.can_at_least(mid)) {
                bestLow = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        lo = minBound;
        hi = maxBound;
        int64 bestHigh = maxBound;
        while (lo <= hi) {
            int64 mid = lo + (hi - lo) / 2;
            if (matcher.can_at_most(mid)) {
                bestHigh = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        std::cout << (bestHigh - bestLow) << '\n';
    }
    return 0;
}
