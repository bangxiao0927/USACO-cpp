#include <iostream>
#include <map>
using namespace std;
using ll = long long;

int main() {
    int N;
    ll A, B;
    cin >> N >> A >> B;
    
    // Map from ID to count
    map<ll, ll> cnt;
    for (int i = 0; i < N; i++) {
        ll FjFavCows, d;
        cin >> FjFavCows >> d;
        cnt[d] = FjFavCows;
    }
    
    ll answer = 0;
    
    // 第一步：处理自配对（2 * d == A 或 2 * d == B）
    for (auto it = cnt.begin(); it != cnt.end(); ++it) {
        ll d = it->first;
        ll num = it->second;
        if (2 * d == A || 2 * d == B) {
            answer += num / 2;
            cnt[d] = num % 2;  // 留下无法配对的单只奶牛
        }
    }
    
    // 第二步：处理跨组配对（d1 + d2 = A 或 B，且 d1 < d2）
    for (auto it = cnt.begin(); it != cnt.end(); ++it) {
        ll d1 = it->first;
        ll num1 = it->second;
        if (num1 == 0) continue;  // 这组已经全部用完
        
        // 检查和为 A 的配对
        ll targetA = A - d1;
        if (targetA > d1) {  // 只处理 d1 < d2，避免重复计算
            auto jt = cnt.find(targetA);
            if (jt != cnt.end()) {
                ll num2 = jt->second;
                ll pairs = min(num1, num2);
                answer += pairs;
                cnt[d1] -= pairs;
                cnt[targetA] -= pairs;
                num1 = cnt[d1];  // 更新 num1 以便检查 B
            }
        }
        
        // 检查和为 B 的配对
        ll targetB = B - d1;
        if (targetB > d1) {
            auto jt = cnt.find(targetB);
            if (jt != cnt.end()) {
                ll num2 = jt->second;
                ll pairs = min(num1, num2);
                answer += pairs;
                cnt[d1] -= pairs;
                cnt[targetB] -= pairs;
            }
        }
    }
    
    cout << answer << '\n';
    return 0;
}