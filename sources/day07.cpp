//
// Created by Gyebro on 2019-12-06.
//
#include <algorithm>
#include "day07.h"
#include "intcode_vm.h"


void day07(bool part_two) {
    cout << "AoC D7: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input07.txt");
    if (in.is_open()) {
        vector<int64_t> program;
        string line;
        while (getline(in, line, ',')) {
            program.push_back(stoi(line));
        }
        if (!part_two) {
            // Part one
            vector<int64_t> phase = {0, 1, 2, 3, 4};
            int final_out;
            int max_final = 0;
            do {
                IntcodeProgram amp0(program);
                IntcodeProgram amp1(program);
                IntcodeProgram amp2(program);
                IntcodeProgram amp3(program);
                IntcodeProgram amp4(program);
                amp0.sendInput({phase[0], 0});
                amp0.run();
                amp1.sendInput({phase[1], amp0.getOutput()});
                amp1.run();
                amp2.sendInput({phase[2], amp1.getOutput()});
                amp2.run();
                amp3.sendInput({phase[3], amp2.getOutput()});
                amp3.run();
                amp4.sendInput({phase[4], amp3.getOutput()});
                amp4.run();
                final_out = amp4.getOutput();
                max_final = max(max_final, final_out);
            } while(std::next_permutation(phase.begin(), phase.end()));
            cout << "Maximum possible thruster signal is: " << max_final << endl;
        } else {
            // Part two
            vector<int64_t> phase = {5,6,7,8,9};
            int final_out;
            int max_final = 0;
            do {
                //cout << phase[0] << phase[1] << phase[2] << phase[3] << phase[4] << '\n';
                IntcodeProgram ampA(program);
                IntcodeProgram ampB(program);
                IntcodeProgram ampC(program);
                IntcodeProgram ampD(program);
                IntcodeProgram ampE(program);
                ampA.sendInput(phase[0]);
                ampB.sendInput(phase[1]);
                ampC.sendInput(phase[2]);
                ampD.sendInput(phase[3]);
                ampE.sendInput(phase[4]);
                size_t loops = 0;
                bool init = true;
                while (!ampE.hasTerminated()) {
                    if (init) {
                        ampA.sendInput(0);
                        init = false;
                    } else {
                        ampA.sendInput(ampE.getOutput());
                    }
                    if (!ampA.hasOutput()) ampA.run(); // Continue running if ampA has no more output
                    ampB.sendInput(ampA.getOutput());
                    if (!ampB.hasOutput()) ampB.run();
                    ampC.sendInput(ampB.getOutput());
                    if (!ampC.hasOutput()) ampC.run();
                    ampD.sendInput(ampC.getOutput());
                    if (!ampD.hasOutput()) ampD.run();
                    ampE.sendInput(ampD.getOutput());
                    if (!ampE.hasOutput()) ampE.run();
                    loops++;
                }
                while (ampE.hasOutput()) { final_out = ampE.getOutput(); }
                //cout << "Output: " << final_out << " after " << loops << " feedback loop\n";
                max_final = max(max_final, final_out);
            } while(std::next_permutation(phase.begin(), phase.end()));
            cout << "Maximum possible thruster signal is: " << max_final << endl;
        }
    }
}