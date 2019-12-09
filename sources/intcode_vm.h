//
// Created by Gyebro on 2019-12-05.
//
#ifndef AOC19_INTCODE_VM_H
#define AOC19_INTCODE_VM_H

#include "common.h"

bool run_intcode_program(vector<int>& o, queue<int>& inputs);

bool run_intcode_program(vector<int>& o, queue<int>& inputs, queue<int>& outputs);

bool run_intcode_program(vector<int>& o);

class IntcodeProgram {
private:
    vector<long int> program;
    queue<long int> inputs;
    queue<long int> outputs;
    size_t p;
    bool halted;
public:
    IntcodeProgram(vector<long int>& intcodes) : program(intcodes) {
        p = 0;
        halted = false;
    };
    void sendInput(const long int& i) {
        inputs.push(i);
    }
    void sendInput(vector<long int> in) {
        for (int i : in) {
            inputs.push(i);
        }
    }
    bool hasOutput() {
        return !outputs.empty();
    }
    long int getOutput() {
        if (!hasOutput()) cout << "Error: trying to get output from empty queue!\n";
        long int out = outputs.front();
        outputs.pop();
        return out;
    }
    long int readOutput() {
        if (!hasOutput()) cout << "Error: trying to get output from empty queue!\n";
        return outputs.front();
    }
    bool hasTerminated() {
        return halted;
    }
    bool run();
};

#endif //AOC19_INTCODE_VM_H
