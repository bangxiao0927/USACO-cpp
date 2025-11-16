#include <fstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace std;

int main() {
    ifstream fin("traffic.in");
    ofstream fout("traffic.out");
    int N,a,b;
    int first_a = 0;
    int first_b = 0;
    string first_ramp;
    int orig_a = 0;
    int orig_b = 0;
    int on_least = 0;
    int on_max = 0;
    int off_least = 0;
    int off_max = 0;
    string ramp;
    bool first_none = true;
    fin >> N;
    fin >> first_ramp >> first_a >> first_b;
    if (first_ramp == "on") {
        on_least += min(first_a,first_b);
        on_max += max(first_a,first_b);
    }
    else if (first_ramp == "none") {
            orig_a = a;
            orig_b = b;
    }
    else if (first_ramp == "off") {
        off_least += min(first_a,first_b);
        off_max += max(first_a,first_b);            
    }
    for (int i=1;i < N;i++) {
        fin >> ramp >> a >> b;
        if (ramp == "on") {
            on_least += min(a,b);
            on_max += max(a,b);
        }
        else if (ramp == "none") {
            if (first_none == true) {
                orig_a = a;
                orig_b = b;
                first_none = false;
            }
            int lingshi_a = orig_a;
            orig_a = max(min(a,b),min(lingshi_a,orig_b));
            orig_b = min(max(a,b),max(lingshi_a,orig_b));
        }
        else if (ramp == "off") {
            off_least += min(a,b);
            off_max += max(a,b);            
        }     
    }
    if (first_ramp == "on") {
        int least_1 = orig_a - max(first_a,first_b);
        int max_1 = orig_b - min(first_a,first_b);
        fout << least_1 << " "<< max_1 << endl;
    }
    else if (first_ramp == "none") {
        fout << orig_a << " " << orig_b << endl;
    }
    else if (first_ramp == "off") {
        int off_least_first = orig_a + first_a;
        int off_max_first = orig_b + first_b;
        fout << off_least_first << " " << off_max_first << endl;
    }
    int least_N = orig_a + on_max - off_max;
    int max_N = orig_b + on_least - off_least;
    cout << least_N << max_N<< orig_a << orig_b;
    fout << least_N << " " << max_N;
}


/*int main() {
    ifstream fin("traffic.in");
    ofstream fout("traffic.out");
    int N;
    fin >> N;
    int noneLMax = 0;
    int noneRMin = 0;
    int increL = 0;
    int increR = 0;
    int beforeL = 0;
    int beforeR = 0;

    for (int i = 0; i < N; i++) {
        string tp;
        int l, r;
        fin >> tp >> l >> r;
        cout << tp << " " << l << " " << r << endl;
        if (tp == "none") {
            beforeL += increL;
            beforeR += increR;
            increL = 0;
            increR = 0;
            if (l > noneLMax) {
                noneLMax = l;
            }
            if (noneRMin == 0 || noneRMin > r) {
                noneRMin = r;
            }
        } else if (tp == "on") {
            increL += l;
            increR += r;
        } else if (tp == "off") {
            increL -= l;
            increR -= r;
        }
        cout << noneLMax << " " << noneRMin << " " << increL << " " << increR << " " << beforeL << " " << beforeR << endl;
    }
    
    fout << noneLMax - beforeR << " " << noneRMin - beforeL << endl;
    fout << noneLMax + increR << " " << noneRMin + increL << endl;
}*/