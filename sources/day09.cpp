//
// Created by Gyebro on 2019-12-09.
//

#include "day09.h"
#include "intcode_vm.h"

void day09(bool part_two) {
    cout << "AoC D9: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input09.txt");
    if (in.is_open()) {
        IntcodeProgram program(in);
        program.reserveMemory(1200);
        program.setVerbose(false);
        if (!part_two) {
            program.sendInput(1);
        } else {
            program.sendInput(2);
        }
        program.run();
        cout << program.readLastOutput() << endl;
    }
}

