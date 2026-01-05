#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int N;
        cin >> N;
        vector<long long> a(N);
        long long total = 0;
        for (int i = 0; i < N; ++i) {
            cin >> a[i];
            total += a[i];
        }

        int len = N / 2 + 1;  // 最终剩余的蛋糕数量
        long long window = 0;
        for (int i = 0; i < len; ++i) window += a[i];
        long long bessie = window;
        for (int i = len; i < N; ++i) {
            window += a[i] - a[i - len];
            bessie = min(bessie, window);
        }

        long long elsie = total - bessie;
        cout << bessie << ' ' << elsie << '\n';
    }

    return 0;
}
