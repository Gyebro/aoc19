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
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;



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
