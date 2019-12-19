//
// Created by Gyebro on 2019-12-19.
//
#include "day19.h"
#include "intcode_vm.h"

void day19(bool part_two) {
    cout << "AoC D19: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input19.txt");
    if (in.is_open()) {
        IntcodeProgram p(in);
        size_t map_size = 50;
        size_t affected = 0;
        if (!part_two) {
            bool print_map = false;
            for (size_t y = 0; y < map_size; y++) {
                for (size_t x = 0; x < map_size; x++) {
                    IntcodeProgram p_inst = p;
                    p_inst.sendInput(x);
                    p_inst.sendInput(y);
                    p_inst.run();
                    if (p_inst.hasOutput()) {
                        int64_t o = p_inst.getOutput();
                        if (o == 1) {
                            affected++;
                        }
                        if (o > 1 || o < 0) {
                            cout << "Error: unexpected output!\n";
                        }
                        if (print_map) { cout << ((o == 1) ? '#' : '.'); }
                    }
                }
                if (print_map) cout << endl;
            }
            cout << "Number of area units affected: " << affected << endl;
        } else {
            // Part Two
            map_size=1000;
            size_t x0=600;
            size_t y0=1000; // based on part 1
            bool finished = false;
            for (size_t y = y0; y < y0 + map_size; y++) {
                for (size_t x = x0; x < x0 + map_size; x++) {
                    IntcodeProgram p_inst = p;
                    p_inst.sendInput(x);
                    p_inst.sendInput(y);
                    p_inst.run();
                    if (p_inst.hasOutput()) {
                        int64_t o = p_inst.getOutput();
                        if (o == 1) {
                            // This is a potential bottom-left corner, check top-right corner
                            IntcodeProgram p_inst2 = p;
                            p_inst2.sendInput(x+99);
                            p_inst2.sendInput(y-99);
                            p_inst2.run();
                            if (p_inst2.getOutput() == 1) {
                                cout << "100 x 100 area found at " << x << ", " << (y-99) << ", checksum: " << 10000*x+y-99 << endl;
                                finished = true;
                            }
                            break;
                        }
                        if (o > 1 || o < 0) {
                            cout << "Error: unexpected output!\n";
                        }
                    }
                }
                if (finished) break;
            }
        }
    }
}

