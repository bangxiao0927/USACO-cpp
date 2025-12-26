#include <fstream>
#include <vector>
using namespace std;

int popcount(long long x) {
    int cnt = 0;
    while (x > 0) {
        cnt += x & 1;
        x >>= 1;
    }
    return cnt;
}

int main() {
    ifstream cin("sequence.in");
    ofstream cout("sequence.out");
    int T;
    long long M;
    int K;
    cin >> T;
    
    while (T--) {
        cin >> M >> K;
        
        int pc_M = popcount(M);
        
        // If K > popcount(M), impossible
        if (K > pc_M) {
            cout << -1 << "\n";
            continue;
        }
        
        // If K == popcount(M), just output M
        if (K == pc_M) {
            cout << 1 << "\n";
            cout << M << "\n";
            continue;
        }
        
        // Strategy: We want sum = M and XOR of popcounts = K
        // Start with M in the sequence: sum = M, XOR = popcount(M)
        // We need XOR to be K, so we need to XOR with (popcount(M) XOR K)
        
        int need = pc_M ^ K;  // What we need to XOR with popcount(M) to get K
        
        vector<long long> result;
        result.push_back(M);
        
        // Add a number with popcount = need, but don't change the sum
        // Solution: Add the number twice (sum += 2*x, XOR unchanged since x XOR x = 0)
        // Then subtract it once more to get back to M
        
        // Actually simpler: Add numbers that sum to 0 but have desired popcount XOR
        // Add 'need' copies of 1: sum increases by 'need', popcount XOR = need (if need is odd) or 0 (if even)
        
        // Better approach:
        // Add one number with popcount 'need', and add it to result
        // But this changes sum by that number
        // To keep sum at M, we can split M differently
        
        if (need == 0) {
            // Already done, XOR is correct
        } else {
            // We need to add 'need' to the XOR
            // Add 'need' ones: popcount(1) = 1, so XOR of 'need' ones = need if odd, 0 if even
            // For need > 0, add a number with popcount = need, then pair it
            
            // Simpler: subtract powers of 2 from M, add them separately
            // If M >= (1 << need), we can split off (1 << need) from M
            long long num = (1LL << need) - 1;  // has popcount = need
            if (M >= num) {
                result[0] = M - num;  // Reduce M
                result.push_back(num);  // Add number with popcount = need
            } else {
                // M is too small, use multiple 1s
                if (M >= need) {
                    result[0] = M - need;
                    for (int i = 0; i < need; i++) {
                        result.push_back(1);
                    }
                } else {
                    cout << -1 << "\n";
                    continue;
                }
            }
        }
        
        // Verify
        long long sum = 0;
        int xor_sum = 0;
        for (long long x : result) {
            sum += x;
            xor_sum ^= popcount(x);
        }
        
        if (sum == M && xor_sum == K && result.size() <= 100) {
            cout << result.size() << "\n";
            for (int i = 0; i < result.size(); i++) {
                if (i > 0) cout << " ";
                cout << result[i];
            }
            cout << "\n";
        } else {
            cout << -1 << "\n";
        }
    }
    
    return 0;
}