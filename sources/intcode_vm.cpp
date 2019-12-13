//
// Created by Gyebro on 2019-12-05.
//
#include "intcode_vm.h"

int64_t& IntcodeProgram::getReference(size_t shift, size_t mode) {
    // Increase memory if needed:
    //(mode == 2) ? reserveMemory(rel_base+program[p+shift]) : \
        ((mode == 0) ? reserveMemory(program[p+shift]) : reserveMemory(p+shift));
    // Return reference
    //return (mode == 2) ? program[rel_base+program[p+shift]] : ((mode == 0) ? program[program[p+shift]] : program[p+shift]);
    if (mode == 0) { // Position mode
        reserveMemory(p+shift);
        reserveMemory(program[p+shift]);
        if (p+shift >= program.size() || p+shift < 0) {
            cout << "Error: out of bounds! p = " << p+shift << endl;
        }
        if (program[p+shift] >= program.size() || program[p+shift] < 0) {
            cout << "Error: out of bounds! o[p] = " << program[p+shift] << endl;
        }
        return program[program[p+shift]];
    } else if (mode == 1) { // Value (immediate) mode
        reserveMemory(p+shift);
        if (p+shift >= program.size() || p+shift < 0) {
            cout << "Error: out of bounds! p = " << p+shift << endl;
        }
        return program[p+shift];
    } else if (mode == 2) { // Relative position mode
        reserveMemory(p+shift);
        reserveMemory(rel_base+program[p+shift]);
        if (p+shift >= program.size() || p+shift < 0) {
            cout << "Error: out of bounds! p = " << p+shift << endl;
        }
        if (rel_base+program[p+shift] >= program.size() || rel_base+program[p+shift] < 0) {
            cout << "Error: out of bounds! o[p] = " << rel_base+program[p+shift] << " (rel_base = " << rel_base << ")" << endl;
        }
        return program[rel_base+program[p+shift]];
    }
    return program[0];
}

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
        // Error check on modes
        if (m0 > 2 || m1 > 2 || m2 > 2) {
            cout << "Error, invalid parameter mode\n";
            return false;
        }
        switch (op) {
            case 1: { // Add p0 and p1 into p2
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                int64_t &p0 = getReference(1, m0);
                int64_t &p1 = getReference(2, m1);
                int64_t &p2 = getReference(3, m2);
                p2 = p0 + p1;
                p += 4;
                break;
            }
            case 2: { // Multiply p0 and p1 into p2
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                int64_t &p0 = getReference(1, m0);
                int64_t &p1 = getReference(2, m1);
                int64_t &p2 = getReference(3, m2);
                p2 = p0 * p1;
                p += 4;
                break;
            }
            case 3: { // Write input into p0
                if (m0 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                int64_t &p0 = getReference(1, m0);
                if (inputs.empty()) {
                    run = false;
                } else {
                    p0 = inputs.front();
                    inputs.pop_front();
                    p += 2;
                }
                break;
            }
            case 4: { // Output p0
                int64_t &p0 = getReference(1, m0);
                if (verbose) cout << "Output: " << p0 << endl;
                outputs.push_back(p0);
                p += 2;
                break;
            }
            case 5: { // Jump if true: if p0!=0, p=p1
                int64_t &p0 = getReference(1, m0);
                int64_t &p1 = getReference(2, m1);
                if (p0 != 0) {
                    p = p1; // Jump
                } else {
                    p += 3;
                }
                break;
            }
            case 6: { // Jump if false: if p0==0, p=p1
                int64_t &p0 = getReference(1, m0);
                int64_t &p1 = getReference(2, m1);
                if (p0 == 0) {
                    p = p1; // Jump
                } else {
                    p += 3;
                }
                break;
            }
            case 7: { // Less than:  p2 = p0<p1
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                int64_t &p0 = getReference(1, m0);
                int64_t &p1 = getReference(2, m1);
                int64_t &p2 = getReference(3, m2);
                if (p0 < p1) {
                    p2 = 1;
                } else {
                    p2 = 0;
                }
                p += 4;
                break;
            }
            case 8: { // Equals:  p2 = p0==p1
                if (m2 == 1) cout << "Error: 'write to' parameter in 'immediate' mode\n";
                int64_t &p0 = getReference(1, m0);
                int64_t &p1 = getReference(2, m1);
                int64_t &p2 = getReference(3, m2);
                if (p0 == p1) {
                    p2 = 1;
                } else {
                    p2 = 0;
                }
                p += 4;
                break;
            }
            case 9: { // Adjust relative base by p0
                int64_t &p0 = getReference(1, m0);
                rel_base += p0;
                p += 2;
                break;
            }
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
