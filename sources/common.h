//
// Created by Gyebro on 2019-12-05.
//

#ifndef AOC19_COMMON_H
#define AOC19_COMMON_H

#define USE_WINDOWS_CLOCK

#ifdef USE_WINDOWS_CLOCK
    #include "windows.h"
#endif

#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <functional>

using namespace std;

#define PI 3.1415926535897932384626433832795

size_t lcm(size_t a, size_t b);

size_t lcm(size_t& a, size_t& b, size_t& c);

const string clear_terminal("\x1B[2J\x1B[H");

template<class T>
bool contains(const vector<T>& c, const T& t) {
    for (const T& e : c) {
        if (e == t) return true;
    }
    return false;
}

template<class T, class U>
size_t find_idx(const T& container, const U& elem) {
    return find(container.begin(), container.end(), elem)-container.begin();
}


template<class T, class Functor>
size_t find_idx_if(const T& container, Functor lambda) {
    for (size_t idx=0; idx<container.size(); idx++) {
        if (lambda(container[idx])) return idx;
    }
    return container.size();
}

class Clock {
private:
#ifndef USE_WINDOWS_CLOCK
    chrono::high_resolution_clock::time_point t_start, t_stop;
#else
    double PCFreq;
    LARGE_INTEGER li;
    __int64 CounterStart, CounterStop;
#endif
public:
    Clock();
    void start();
    void stop();
    double read_sec();
    double read_msec();
};

#endif //AOC19_COMMON_H
