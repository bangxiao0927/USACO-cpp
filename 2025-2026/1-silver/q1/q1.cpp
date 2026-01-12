#include <algorithm>
#include <iostream>

using namespace std;

using int64 = long long;

int64 position_of_cow(int64 c, int64 t) {
    int64 limit_len = t / 2 + 1;

    if (c >= limit_len) return c;

    int64 pos = c;
    int64 ell = max<int64>(2, c + 1);
    int64 limit = (t & 1) ? limit_len : limit_len - 1;

    while (ell <= limit) {
        if (pos >= 2) {
            int64 steps = pos / 2;
            int64 remaining = limit - ell + 1;
            if (steps > remaining) steps = remaining;
            pos -= 2 * steps;
            ell += steps;
        } else {
            pos = ell - (2 - pos);
            ++ell;
        }
    }

    if ((t & 1) == 0) {
        pos = (pos == 0) ? (limit_len - 1) : (pos - 1);
    }

    return pos;
}

int64 cow_at_position(int64 x, int64 t) {
    int64 limit_len = t / 2 + 1;

    if (x >= limit_len) return x;

    int64 pos = x;
    int64 ell;

    if ((t & 1) == 0) {
        pos = (pos + 1) % limit_len;
        ell = limit_len - 1;
    } else {
        ell = limit_len;
    }

    while (ell > 1 && pos < ell) {
        int64 delta = ell - pos;
        if (delta <= 2) {
            pos += 2;
            if (pos >= ell) pos -= ell;
            --ell;
        } else {
            int64 steps = delta / 3;
            if (steps <= 0) steps = 1;
            pos += 2 * steps;
            ell -= steps;
        }
    }

    return pos;
}

int main() {
    int Q;
    cin >> Q;
    
    while (Q--) {
        int type;
        cin >> type;
        int64 a, b;
        cin >> a >> b;
        if (type == 1) {
            cout << position_of_cow(a, b) << '\n';
        } else {
            cout << cow_at_position(a, b) << '\n';
        }
    }
    return 0;
}
