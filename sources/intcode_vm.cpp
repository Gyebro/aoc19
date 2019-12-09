//
// Created by Gyebro on 2019-12-05.
//
#include "intcode_vm.h"

bool IntcodeProgram::run() {
    int64_t op;
    int64_t m0, m1, m2;
    bool run = true;
    if (halted) {
        cout << "Error: trying to run halted VM\n";
    }
    while (!halted && run) {
        // Decompose opcode
        op = program[p];
        m2 = op / 10000; op -= m2 * 10000;
        m1 = op / 1000; op -= m1 * 1000;
        m0 = op / 100; op -= m0 * 100;
        // TODO: Check if we have enough memory
        // Modes: 2: relative, 0: position, 1: immediate
        int64_t& p0 = (m0 == 2) ? program[rel_base+program[p + 1]] : ((m0 == 0) ? program[program[p + 1]] : program[p + 1]);
        int64_t& p1 = (m1 == 2) ? program[rel_base+program[p + 2]] : ((m1 == 0) ? program[program[p + 2]] : program[p + 2]);
        int64_t& p2 = (m2 == 2) ? program[rel_base+program[p + 3]] : ((m2 == 0) ? program[program[p + 3]] : program[p + 3]);
        // Error check on modes
        if (m0 > 2 || m1 > 2 || m2 > 2) {
            cout << "Error, invalid parameter mode\n";
            return false;
        }
        switch (op) {
            case 1: // Add p0 and p1 into p2
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                p2 = p0 + p1;
                p+=4;
                break;
            case 2: // Multiply p0 and p1 into p2
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                p2 = p0 * p1;
                p+=4;
                break;
            case 3: // Write input into p0
                if (m0 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                if (inputs.empty()) {
                    run = false;
                } else {
                    p0 = inputs.front();
                    inputs.pop();
                    p+=2;
                }
                break;
            case 4: // Output p0
                if (verbose) cout << "Output: " << p0 << endl;
                outputs.push(p0);
                p+=2;
                break;
            case 5: // Jump if true: if p0!=0, p=p1
                if (p0 != 0) {
                    p = p1; // Jump
                } else {
                    p += 3;
                }
                break;
            case 6: // Jump if false: if p0==0, p=p1
                if (p0 == 0) {
                    p = p1; // Jump
                } else {
                    p += 3;
                }
                break;
            case 7: // Less than:  p2 = p0<p1
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                if (p0 < p1) {
                    p2 = 1;
                } else {
                    p2 = 0;
                }
                p += 4;
                break;
            case 8: // Equals:  p2 = p0==p1
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                if (p0 == p1) {
                    p2 = 1;
                } else {
                    p2 = 0;
                }
                p += 4;
                break;
            case 9: // Adjust relative base by p0
                rel_base += p0;
                p += 2;
                break;
            case 99:
                if (verbose) cout << "Intcode program terminated successfully\n";
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
