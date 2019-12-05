//
// Created by Gyebro on 2019-12-05.
//
#include "day01.h"

int fuel(int mass) {
    int ret = 0;
    int f = 0;
    f = mass/3-2; // fuel amount required to transport mass
    while (f > 0) {
        ret += f;
        f = f/3-2; // fuel amount required to transport itself
    }
    return ret;
}

void day01(bool part_two) {
    cout << "AoC D1: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input01.txt");
    if (in.is_open()) {
        int mass;
        long int total = 0;
        while (in >> mass) {
            if (!part_two) {
                total += mass/3-2;
            } else {
                total += fuel(mass);
            }
        }
        cout << total << endl;
    }
}

