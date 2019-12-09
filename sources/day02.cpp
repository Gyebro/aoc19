//
// Created by Gyebro on 2019-12-05.
//
#include "day02.h"
#include "intcode_vm.h"

void day02(bool part_two) {
    cout << "AoC D2: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input02.txt");
    if (in.is_open()) {
        vector<int64_t> program;
        string line;
        while (getline(in, line, ',')) {
            program.push_back(stoll(line));
        }
        if (!part_two) {
            program[1] = 12;
            program[2] = 2;
            IntcodeProgram p(program);
            p.run();
            cout << "reg[0] = " << p.readReg(0) << endl;
        } else {
            int target = 19690720;
            vector<int64_t> program_copy(program);
            program_copy[1] = 12;
            program_copy[2] = 2;
            // Get initial result
            IntcodeProgram p(program_copy);
            p.run();
            int initial = p.readReg(0);
            // See what difference +1 in noun makes
            program_copy = program;
            program_copy[1] = 13;
            program_copy[2] = 2;
            IntcodeProgram p2(program_copy);
            p2.run();
            int noun_diff = p2.readReg(0)-initial;
            // See what difference +1 in verb makes
            program_copy = program;
            program_copy[1] = 12;
            program_copy[2] = 3;
            IntcodeProgram p3(program_copy);
            p3.run();
            int verb_diff = p3.readReg(0)-initial;
            cout << "Diff caused by noun++ is " << noun_diff << endl;
            cout << "Diff caused by verb++ is " << verb_diff << endl;
            int noun_inc = (target-initial)/noun_diff;
            int verb_inc = (target-initial-noun_inc*noun_diff)/verb_diff;
            cout << "Target noun is " << 12+noun_inc << endl;
            cout << "Target verb is " << 2+verb_inc << endl;
            cout << "Solution is " << (12+noun_inc)*100 + (2+verb_inc) << endl;
        }
    }
}