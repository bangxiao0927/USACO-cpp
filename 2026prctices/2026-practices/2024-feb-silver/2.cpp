#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Compress consecutive identical digits so each run is represented once.
std::vector<int> compress_runs(const std::string& s) {
    std::vector<int> res;
    res.reserve(s.size());
    for (char ch : s) {
        int val = ch - '0';
        if (res.empty() || res.back() != val) {
            res.push_back(val);
        }
    }
    return res;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int T;
    if (!(std::cin >> T)) return 0;
    while (T--) {
        int N, P;
        std::cin >> N >> P;
        std::vector<std::vector<int>> tubes(3);
        for (int i = 0; i < 2; ++i) {
            std::string s;
            std::cin >> s;
            tubes[i] = compress_runs(s);
        }

        std::vector<std::pair<int, int>> moves;
        // Helper to record a move and mutate the compressed stacks accordingly.
        auto add_move = [&](int src, int dst) {
            moves.emplace_back(src, dst);
            if (tubes[dst].empty() || tubes[dst].back() != tubes[src].back()) {
                tubes[dst].push_back(tubes[src].back());
            }
            tubes[src].pop_back();
        };

        // Ensure the first two tubes begin with different colors by cloning a dummy run.
        if (tubes[0][0] == tubes[1][0]) {
            tubes[0].insert(tubes[0].begin(), tubes[0][0] ^ 3);
        }

        long long ans = static_cast<long long>(tubes[0].size() + tubes[1].size()) - 2;
        if (ans < 0) ans = 0;
        if (ans > 1) ++ans;

        std::cout << ans << '\n';
        if (P == 1) continue;

        // If both top runs match, peel from the taller stack to keep colors distinct.
        if (tubes[0].back() == tubes[1].back()) {
            if (tubes[0].size() > tubes[1].size()) {
                add_move(0, 1);
            } else {
                add_move(1, 0);
            }
        }

        // Drain the tube until one run remains, using the auxiliary tube to store matches.
        auto empty_until_one = [&](int idx) {
            while (tubes[idx].size() > 1) {
                if (tubes[idx].back() == tubes[2][0]) {
                    add_move(idx, 2);
                } else {
                    add_move(idx, idx ^ 1);
                }
            }
        };

        for (int i = 0; i < 2; ++i) {
            if (tubes[i].size() > 1) {
                add_move(i, 2);
                int idx = 0;
                if (tubes[idx][0] == tubes[2][0]) idx ^= 1;
                empty_until_one(idx);
                idx ^= 1;
                empty_until_one(idx);
                add_move(2, idx);
                break;
            }
        }

        assert(static_cast<long long>(moves.size()) == ans);
        for (const auto& mv : moves) {
            std::cout << (mv.first + 1) << ' ' << (mv.second + 1) << '\n';
        }
    }
    return 0;
}
