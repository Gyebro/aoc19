//
// Created by Gyebro on 2019-12-05.
//
#ifndef AOC19_INTCODE_VM_H
#define AOC19_INTCODE_VM_H

#include "common.h"

class IntcodeProgram {
private:
    vector<int64_t> program;
    queue<int64_t> inputs;
    deque<int64_t> outputs;
    size_t p;
    size_t rel_base;
    bool halted;
    bool verbose;
    int64_t& getReference(size_t pos, size_t mode);
public:
    IntcodeProgram(ifstream& input) : p(0), rel_base(0), halted(false), verbose(false) {
        string line;
        while (getline(input, line, ',')) {
            program.push_back(stoll(line));
        }
    }
    IntcodeProgram(vector<int64_t>& intcodes) : program(intcodes), p(0), rel_base(0), halted(false),
        verbose(false) {
    };
    void sendInput(const int64_t& i) {
        inputs.push(i);
    }
    void sendInput(vector<int64_t> in) {
        for (int i : in) {
            inputs.push(i);
        }
    }
    bool hasOutput() {
        return !outputs.empty();
    }
    int64_t getOutput() {
        if (!hasOutput()) cout << "Error: trying to get output from empty queue!\n";
        int64_t out = outputs.front();
        outputs.pop_front();
        return out;
    }
    int64_t readOutput() {
        if (!hasOutput()) cout << "Error: trying to get output from empty queue!\n";
        return outputs.front();
    }
    int64_t readLastOutput() {
        if (!hasOutput()) cout << "Error: trying to get output from empty queue!\n";
        return outputs.back();
    }
    bool hasTerminated() {
        return halted;
    }
    bool run();
    int64_t readReg(uint64_t pos) const {
        return program[pos];
    }
    void writeReg(uint64_t pos, int64_t val) {
        program[pos]=val;
    }
    void reserveMemory(uint64_t memsize) {
        if (program.size() < memsize+1) {
            if (verbose) cout << "Increasing memory to " << memsize << endl;
            program.resize(memsize+1,0);
        }
    }
    void setVerbose(bool verb) {
        verbose = verb;
    }
};

#endif //AOC19_INTCODE_VM_H
