//
// Created by Gyebro on 2019-12-23.
//
#include "day22.h"

void dns(const size_t N, size_t& n) {
    n = (N-1)-n;
}

void cut(const size_t N, size_t& n, const int a) {
    if (a>0) { //cut positive
        size_t m=N-a;
        if (n<a) { // move back by m
            n+=m;
        } else { // move fwd by a
            n-=a;
        }
    } else { //cut neg
        size_t m=N+a; // m<N
        if (n<m) { // move back by a
            n-=a;
        } else { // move fwd by m
            n-=m;
        }
    }
}

void dwi(const size_t N, size_t& n, const size_t i) {
    n = (i*n)%N;
}

void day22(bool part_two) {
    cout << "AoC D22: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input22.txt");
    if (in.is_open()) {
        string line;
        int n;
        if (!part_two) {
            const size_t N = 10007;
            size_t idx = 2019;
            while(getline(in,line)) {
                if (line.substr(0,3)=="cut") {
                    n=stoi(line.substr(4));
                    cut(N,idx,n);
                } else if (line.substr(5,4)=="into") {
                    dns(N,idx);
                } else if (line.substr(5,4)=="with") {
                    n=stoi(line.substr(20));
                    dwi(N,idx,n);
                }
            }
            cout << "index of 2019 after shuffle: " << idx << endl;
        } else {
            cout << "TODO!" << endl;
        }
    }
}

