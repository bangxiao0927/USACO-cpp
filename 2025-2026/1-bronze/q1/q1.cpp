#include <algorithm>
#include <cstdint>
#include <climits>
#include <iostream>
#include <limits>
#include <vector>

using int64 = long long;
using i128 = __int128_t;

static const int64 MAX_X = 1000000000000000000LL;

static void addCandidate(std::vector<int64>& candidates, int64 minB, int64 maxB, int64 value) {
    if (value < minB || value > maxB) {
        return;
    }
    candidates.push_back(value);
}

static i128 computeWorst(int64 A, int64 B, int64 cA, int64 cB, int64 x) {
    int64 minB = B;
    int64 maxB = B + x;
    std::vector<int64> candidates;

    addCandidate(candidates, minB, maxB, minB);
    addCandidate(candidates, minB, maxB, maxB);
    if (cB > 1) {
        int64 remLow = minB % cB;
        int64 delta = (cB - 1) - remLow;
        int64 firstTop = minB + delta;
        addCandidate(candidates, minB, maxB, firstTop);

        if (maxB >= cB - 1) {
            int64 remHigh = (maxB - (cB - 1)) % cB;
            int64 lastTop = maxB - remHigh;
            addCandidate(candidates, minB, maxB, lastTop);
        }
    }

    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

    i128 best = (i128)4 * (i128)LLONG_MAX;
    for (size_t i = 0; i < candidates.size(); ++i) {
        int64 value = candidates[i];
        i128 q = value / cB;
        i128 res = (i128)(A + B) + x - value + (i128)cA * q;
        if (res < best) {
            best = res;
        }
    }
    return best;
}

static bool canReach(int64 A, int64 B, int64 cA, int64 cB, int64 fA, int64 x) {
    return computeWorst(A, B, cA, cB, x) >= (i128)fA;
}

// Binary searches the smallest x that keeps the worst-case total at or above fA.
static int64 findMinimumX(int64 A, int64 B, int64 cA, int64 cB, int64 fA) {
    if (canReach(A, B, cA, cB, fA, 0)) {
        return 0;
    }

    int64 hi = 1;
    while (!canReach(A, B, cA, cB, fA, hi)) {
        if (hi >= MAX_X) {
            break;
        }
        hi = std::min(hi * 2, MAX_X);
    }

    int64 lo = 0;
    while (lo < hi) {
        int64 mid = lo + (hi - lo) / 2;
        if (canReach(A, B, cA, cB, fA, mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

int main() {
    int T;
    if (!(std::cin >> T)) {
        return 0;
    }
    std::vector<int64> answers;
    answers.reserve(T);
    while (T--) {
        int64 A, B, cA, cB, fA;
        std::cin >> A >> B >> cA >> cB >> fA;
        int64 best = findMinimumX(A, B, cA, cB, fA);
        answers.push_back(best);
    }
    for (size_t i = 0; i < answers.size(); ++i) {
        std::cout << answers[i] << '\n';
    }
    return 0;
}