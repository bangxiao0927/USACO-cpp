#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>

void score_judge(const char* input) {
    int scores_A = 0;
    int scores_B = 0;
    for (int i=0;i< strlen(input);i+=2) {
        if (input[i] == 'A') scores_A += input[i+1] - '0';
        else  scores_B += input[i+1] - '0';
        if (abs(scores_A - scores_B) >= 2 && scores_A >= 11) {
            printf("A\n");
            return;
        } else if (abs(scores_A - scores_B) >= 2 && scores_B >= 11) {
            printf("B\n");
            return;
        }
    }
}

int main() {
    std::string str;
    std::cin >> str;
    const char* data_test = str.c_str();
    score_judge(data_test);
    return 0;
}