#include <fstream>
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

int pour_100(int a,int b,int c,
 int amount_a, int amount_b,int amount_c) {
    ofstream fout("mixmilk.out");
    for (int i=0;i<100;i++) {
        if (i % 3 == 0) {
            if (amount_a + amount_b <= b) {
                amount_b += amount_a;
                amount_a = 0;
            }
            else {
                amount_a = amount_a - (b - amount_b);
                amount_b = b;
            }
        }
        else if (i % 3 == 1) {
            if (amount_b + amount_c <= c) {
                amount_c += amount_b;
                amount_b = 0;
            }
            else {
                amount_b = amount_b - (c - amount_c);
                amount_c = c;
            }
        }
        else {
            if (amount_c + amount_a <= a) {
                amount_a += amount_c;
                amount_c = 0;
            }
            else {
                amount_c = amount_c - (a - amount_a);
                amount_a = a;
            }
        }
    }
    fout << amount_a << endl << amount_b << endl << amount_c;
    return 0;
}

int main() {
    ifstream fin("mixmilk.in");
    int a,b,c,amount_a,amount_b,amount_c;
    fin >> a >> amount_a >> b >> amount_b >> c >> amount_c;
    pour_100(a,b,c,amount_a,amount_b,amount_c);
}