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
    queue<int64_t> outputs;
    size_t p;
    bool halted;
public:
    IntcodeProgram(ifstream& input) : p(0), halted(false) {
        string line;
        while (getline(input, line, ',')) {
            program.push_back(stoll(line));
        }
    }
    IntcodeProgram(vector<int64_t>& intcodes) : program(intcodes), p(0), halted(false) {
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
        outputs.pop();
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
    int64_t readReg(uint64_t pos) {
        return program[pos];
    }
};

#endif //AOC19_INTCODE_VM_H
