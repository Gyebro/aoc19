//
// Created by Gyebro on 2019-12-05.
//
#include "day05.h"
#include "intcode_vm.h"

void day05(bool part_two) {
    cout << "AoC D5: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input05.txt");
    if (in.is_open()) {
        vector<int> program;
        string line;
        while (getline(in, line, ',')) {
            program.push_back(stoi(line));
        }
        cout << program.size() << " intcodes found\n";
        queue<int> inputs;
        if (!part_two) {
            inputs.push(1);
        } else {
            inputs.push(5);
        }
        bool res = run_intcode_program(program, inputs);
    }
}

