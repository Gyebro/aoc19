//
// Created by Gyebro on 2019-12-21.
//
#include "day21.h"
#include "intcode_vm.h"

void day21(bool part_two) {
    cout << "AoC D21: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input21.txt");
    bool interactive_pt1 = false;
    bool interactive_pt2 = false;
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
                string springscript = "NOT C J\n"
                                      "AND D J\n"
                                      "NOT A T\n"
                                      "OR T J\n"
                                      "WALK\n";
                if (interactive_pt1) {
                    getline(cin,input); input+='\n';
                } else {
                    input = springscript;
                    cout << springscript;
                }
                program.sendInputString(input);
                program.run();
            } else {
                // Part Two
                string springscript2 = "OR A J\n"
                                         "AND B J\n"
                                         "AND C J\n"
                                         "NOT J J\n"
                                         "AND D J\n"
                                         "OR E T\n"
                                         "OR H T\n"
                                         "AND T J\n"
                                         "RUN\n";
                if (interactive_pt2) {
                    getline(cin,input); input+='\n';
                } else {
                    cout << springscript2;
                    input = springscript2;
                }
                program.sendInputString(input);
                program.run();
            }
        }
        //cout << "Program terminated\n";
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
