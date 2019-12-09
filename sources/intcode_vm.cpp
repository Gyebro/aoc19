//
// Created by Gyebro on 2019-12-05.
//
#include "intcode_vm.h"

bool IntcodeProgram::run() {
    int64_t op;
    int64_t mode_p0, mode_p1, mode_p2;
    int64_t p0, p1, p2;
    bool run = true;
    if (halted) {
        cout << "Error: trying to run halted VM\n";
    }
    while (!halted && run) {
        // Decompose opcode
        op = program[p];
        mode_p2 = op/10000; op -= mode_p2*10000;
        mode_p1 = op/1000; op -= mode_p1*1000;
        mode_p0 = op/100; op -= mode_p0*100;
        // Error check on modes
        if (mode_p0 > 1 || mode_p1 > 1 || mode_p2 > 1) {
            cout << "Error, invalid parameter mode\n";
            return false;
        }
        switch (op) {
            case 1: // Add arg1 and arg2 into arg3
                p0 = (mode_p0 == 0) ? program[program[p + 1]] : program[p + 1];
                p1 = (mode_p1 == 0) ? program[program[p + 2]] : program[p + 2];
                if (mode_p2 != 0) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                program[program[p + 3]] = p0 + p1;
                p+=4;
                break;
            case 2: // Multiply arg1 and arg2 into arg3
                p0 = (mode_p0 == 0) ? program[program[p + 1]] : program[p + 1];
                p1 = (mode_p1 == 0) ? program[program[p + 2]] : program[p + 2];
                if (mode_p2 != 0) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                program[program[p + 3]] = p0 * p1;
                p+=4;
                break;
            case 3: // Write input into arg1
                if (mode_p0 != 0) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                if (inputs.empty()) {
                    //cout << "Info: no more input!\n";
                    run = false;
                } else {
                    program[program[p + 1]] = inputs.front();
                    inputs.pop();
                    p+=2;
                }
                break;
            case 4: // Output arg1
                p0 = (mode_p0 == 0) ? program[program[p + 1]] : program[p + 1];
                //cout << "Output: " << p0 << endl;
                outputs.push(p0);
                p+=2;
                break;
            case 5: // Jump if true: if arg1!=0, p=arg2
                p0 = (mode_p0 == 0) ? program[program[p + 1]] : program[p + 1];
                p1 = (mode_p1 == 0) ? program[program[p + 2]] : program[p + 2];
                if (p0 != 0) {
                    p = p1; // Jump
                } else {
                    p += 3;
                }
                break;
            case 6: // Jump if false: if arg1!=0, p=arg2
                p0 = (mode_p0 == 0) ? program[program[p + 1]] : program[p + 1];
                p1 = (mode_p1 == 0) ? program[program[p + 2]] : program[p + 2];
                if (p0 == 0) {
                    p = p1; // Jump
                } else {
                    p += 3;
                }
                break;
            case 7: // Less than:  arg3 = arg1<arg2
                p0 = (mode_p0 == 0) ? program[program[p + 1]] : program[p + 1];
                p1 = (mode_p1 == 0) ? program[program[p + 2]] : program[p + 2];
                if (mode_p2 != 0) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                if (p0 < p1) {
                    program[program[p + 3]] = 1;
                } else {
                    program[program[p + 3]] = 0;
                }
                p += 4;
                break;
            case 8: // Equals:  arg3 = arg1==arg2
                p0 = (mode_p0 == 0) ? program[program[p + 1]] : program[p + 1];
                p1 = (mode_p1 == 0) ? program[program[p + 2]] : program[p + 2];
                if (mode_p2 != 0) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                if (p0 == p1) {
                    program[program[p + 3]] = 1;
                } else {
                    program[program[p + 3]] = 0;
                }
                p += 4;
                break;
            case 99:
                //cout << "Program terminated successfully!\n";
                halted = true;
                run = false;
                break;
            default:
                cout << "Error: unsupported Intcode found\n";
                return false;
        }
    }
    return true;
}
