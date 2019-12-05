//
// Created by Gyebro on 2019-12-05.
//
#include "common.h"

#ifndef USE_WINDOWS_CLOCK

Clock::Clock() {
    start();
}

void Clock::start() {
    t_start = chrono::high_resolution_clock::now();
}

void Clock::stop() {
    t_stop = chrono::high_resolution_clock::now();
}

double Clock::read_sec() {
    auto dt = t_stop - t_start;
    return chrono::duration_cast<std::chrono::milliseconds>(dt).count()/1000.0;
}

double Clock::read_msec() {
    auto dt = t_stop - t_start;
    return chrono::duration_cast<std::chrono::microseconds>(dt).count()/1000.0;
}

#else

Clock::Clock() {
    QueryPerformanceFrequency(&li);
    PCFreq = double(li.QuadPart)/1000.0;
    start();
}

void Clock::start() {
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

void Clock::stop() {
    QueryPerformanceCounter(&li);
    CounterStop = li.QuadPart;
}

double Clock::read_sec() {
    return read_msec()/1000.0;
}

double Clock::read_msec() {
    return static_cast<double>(CounterStop-CounterStart)/PCFreq;
}


#endif
