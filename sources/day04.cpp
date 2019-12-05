//
// Created by Gyebro on 2019-12-05.
//

#include "day04.h"

bool isValid(int p, bool part_two = false) {
    vector<size_t> d(6);
    d[0] = p/100000; p -= d[0]*100000;
    d[1] =  p/10000; p -= d[1]*10000;
    d[2] =   p/1000; p -= d[2]*1000;
    d[3] =    p/100; p -= d[3]*100;
    d[4] =     p/10; p -= d[4]*10;
    d[5] =     p%10;
    // check order
    bool ord = true;
    for (int j=0; j<5; j++) {
        if (d[j] > d[j+1]) {
            ord = false;
            break;
        }
    }
    if (!ord) return false; // Password already invalid
    // check neighbouring digits
    bool neigh = false;
    if (!part_two) {
        for (int j=0; j<5; j++) {
            if (d[j] == d[j+1]) {
                neigh = true;
                break;
            }
        }
    } else {
        bool candidate;
        for (int j=0; j<5; j++) {
            if (d[j] == d[j+1]) {
                candidate = true; // Pair found
                if (j>0 && d[j-1] == d[j]) candidate=false; // check lead-in
                if (j<4 && d[j+2] == d[j]) candidate=false; // check trail
                if (candidate) neigh = true; // Note: not clearing neighbour-check flag if candidate fails!
            }
        }
    }
    return neigh;
}

void day04(bool part_two) {
    cout << "AoC D4: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input04.txt");
    int range_min, range_max;
    if (in.is_open()) {
        in >> range_min >> range_max;
        range_max = abs(range_max);
        size_t valid = 0;
        for (int i = range_min; i <= range_max; i++) {
            if (isValid(i, part_two)) valid++;
        }
        cout << "Valid passcodes in range: " << valid << endl;
    }
}
