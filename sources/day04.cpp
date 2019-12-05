//
// Created by Gyebro on 2019-12-05.
//

#include "day04.h"

vector<size_t> d(6);

bool isValid(int p, bool part_two = false) {
    for (size_t i=0; i<6; i++) {
        d[5-i]=p%10;
        p/=10;
    }
    // check order
    bool ord = true;
    bool neigh = false;
    int j;
    bool candidate;
    for (j=0; j<5; j++) {
        if (d[j] > d[j+1]) {
            ord = false;
            break;
        }
        if (!part_two) { // Check for at least one pair
            if (d[j] == d[j+1]) {
                neigh = true;
            }
        } else { // Check for at least one pair without same neighbouring digits
            if (d[j] == d[j+1]) {
                candidate = true; // Pair found
                if (j>0 && d[j-1] == d[j]) candidate=false; // check lead-in
                if (j<4 && d[j+2] == d[j]) candidate=false; // check trail
                if (candidate) neigh = true; // Note: not clearing neighbour-check flag if candidate fails!
            }

        }
    }
    return neigh && ord;
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
