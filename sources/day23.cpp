//
// Created by Gyebro on 2019-12-23.
//
#include "day23.h"
#include "intcode_vm.h"

struct cat6_packet {
    int64_t address;
    int64_t msg_x;
    int64_t msg_y;
};

const int64_t flag_no_input = -1;

void day23(bool part_two) {
    cout << "AoC D23: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input23.txt");
    if (in.is_open()) {
        IntcodeProgram computer(in);
        computer.reserveMemory(5000);
        computer.setVerbose(false);
        vector<IntcodeProgram> network(50, computer);
        vector<bool> no_input_state(50,false);
        vector<bool> idle_state(50, false);
        int64_t address = 0;
        // Set up network
        for (IntcodeProgram& pc : network) {
            pc.sendInput(address);
            address++;
        }
        list<cat6_packet> packets;
        cat6_packet p;
        cat6_packet NAT;
        int64_t last_nat_y = -9999;
        bool running = true;
        while (running) {
            // Gather packets
            for (size_t addr = 0; addr < 50; addr++) {
                if (!network[addr].hasOutput() && no_input_state[addr]) {
                    idle_state[addr] = true;
                }
                while (network[addr].hasOutput()) {
                    idle_state[addr] = false;
                    // Try to read packet at once
                    p.address = network[addr].getOutput();
                    p.msg_x   = network[addr].getOutput();
                    p.msg_y   = network[addr].getOutput();
                    if (p.address == 255) {
                        if (!part_two) {
                            cout << "Packet sent: [" << p.address << " | " << p.msg_x << ", " << p.msg_y << "]\n";
                            return;
                        } else {
                            // Part two
                            NAT = p;
                        }
                    } else {
                        packets.push_back(p);
                        //cout << "Packet sent: [" << p.address << " | " << p.msg_x << ", " << p.msg_y << "]\n";
                        //cout << flush;
                    }
                }
            }
            // Send inputs and run
            for (size_t addr = 0; addr < 50; addr++) {
                bool no_input = true;
                if (packets.size() > 0) {
                    auto it = packets.begin();
                    while (it != packets.end()) {
                        if ((*it).address == addr) {
                            //cout << "Delivering [" << (*it).address << " | " << (*it).msg_x << ", " << (*it).msg_y << "]\n";
                            //cout << flush;
                            network[addr].sendInput(it->msg_x);
                            network[addr].sendInput(it->msg_y);
                            it = packets.erase(it);
                            no_input = false;
                        } else {
                            it++;
                        }
                    }
                }
                if (no_input) {
                    network[addr].sendInput(flag_no_input);
                }
                no_input_state[addr] = no_input;
                network[addr].run();
            }
            bool network_idle = true;
            for (const bool b : idle_state) {
                network_idle &= b;
            }
            if (network_idle) {
                // Network idle, send last NAT packet to addr 0
                //cout << "NAT wakes network [" << NAT.msg_x << ", " << NAT.msg_y << "]\n";
                if (last_nat_y == NAT.msg_y) {
                    cout << "Current NAT.y is the same as previous: " << NAT.msg_y << endl;
                    return;
                }
                last_nat_y = NAT.msg_y;
                network[0].sendInput(NAT.msg_x);
                network[0].sendInput(NAT.msg_y);
                idle_state[0] = false;
                no_input_state[0] = false;
            }
        }
    }
}

