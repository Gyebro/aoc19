//
// Created by Gyebro on 2019-12-31.
//
#include "day25.h"
#include "intcode_vm.h"

void day25(bool part_two) {
    cout << "AoC D25: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input25.txt");
    if (part_two) return;
    bool interactive_pt1 = true;
    if (in.is_open()) {
        IntcodeProgram program(in);
        program.reserveMemory(10000);
        program.run();
        while(!program.hasTerminated()) {
            // Display output
            while(program.hasOutput()) {
                cout << static_cast<char>(program.getOutput());
            }
            cout << flush;
            string input;
            if (!part_two) {
                if (interactive_pt1) {
                    getline(cin,input);
                    if (input=="n") input="north";
                    if (input=="s") input="south";
                    if (input=="e") input="east";
                    if (input=="w") input="west";
                    input+='\n';
                } else {
                    cout<<"Auto mode not supported yet!";
                }
                program.sendInputString(input);
                program.run();
            } else {
                // Day 25 doesn't have 2nd part
            }
        }
        // cout << "Program terminated\n";
        while(program.hasOutput()) {
            int64_t o = program.getOutput();
            if ((0 <= o) && (o <= 256)) {
                cout << static_cast<char>(o);
            } else {
                cout << "Return code: " << o << endl;
            }
        }
    }
}