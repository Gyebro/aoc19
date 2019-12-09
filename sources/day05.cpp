//
// Created by Gyebro on 2019-12-05.
//
#include "day05.h"
#include "intcode_vm.h"

void day05(bool part_two) {
    cout << "AoC D5: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input05.txt");
    if (in.is_open()) {
        IntcodeProgram program(in);
        if (!part_two) {
            program.sendInput(1);
        } else {
            program.sendInput(5);
        }
        program.run();
        cout << program.readLastOutput() << endl;
    }
}

