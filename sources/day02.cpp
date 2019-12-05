//
// Created by Gyebro on 2019-12-05.
//
#include "day02.h"
#include "intcode_vm.h"

void day02(bool part_two) {
    cout << "AoC D2: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input02.txt");
    if (in.is_open()) {
        vector<int> program;
        string line;
        while (getline(in, line, ',')) {
            program.push_back(stoi(line));
        }
        cout << program.size() << " intcodes found\n";
        if (!part_two) {
            program[1] = 12;
            program[2] = 2;
            run_intcode_program(program);
            cout << "reg[0] = " << program[0] << endl;
        } else {
            int target = 19690720;
            vector<int> program_copy(program);
            program_copy[1] = 12;
            program_copy[2] = 2;
            // Get initial result
            run_intcode_program(program_copy);
            int initial = program_copy[0];
            // See what difference +1 in noun makes
            program_copy = program;
            program_copy[1] = 13;
            program_copy[2] = 2;
            run_intcode_program(program_copy);
            int noun_diff = program_copy[0]-initial;
            // See what difference +1 in verb makes
            program_copy = program;
            program_copy[1] = 12;
            program_copy[2] = 3;
            run_intcode_program(program_copy);
            int verb_diff = program_copy[0]-initial;
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