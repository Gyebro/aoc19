//
// Created by Gyebro on 2019-12-13.
//
#include "day13.h"
#include "intcode_vm.h"

class output_packet {
public:
    int64_t x;
    int64_t y;
    int64_t c;
    output_packet() : x(0), y(0), c(0) {}
    output_packet(int64_t X, int64_t Y, int64_t C) : x(X), y(Y), c(C) {}
};

const string cTileChars = " +#=o";
const int64_t cJoyNeutral = 0;
const int64_t cJoyLeft = -1;
const int64_t cJoyRight = +1;
const size_t cEmpty = 0;
const size_t cWall = 1;
const size_t cBlock = 2;
const size_t cPaddle = 3;
const size_t cBall = 4;

void day13(bool part_two) {
    cout << "AoC D13: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input13.txt");
    if (in.is_open()) {
        IntcodeProgram program(in);
        program.reserveMemory(5000);
        if (!part_two) { // Part One
            while (!program.hasTerminated()) {
                program.run();
            }
            vector<int64_t> outputs;
            size_t i=0;
            size_t blocks = 0;
            while (program.hasOutput()) {
                i++;
                int64_t o = program.getOutput();
                if (i % 3 == 0) {
                    switch (o) {
                        case cEmpty:// Empty
                        case cWall:// Wall
                        case cPaddle:// Paddle
                        case cBall:// Ball
                            break;
                        case cBlock:// Block
                            blocks++;
                            break;
                        default:
                            cout << "Error: Invalid output!\n";
                            break;
                    }
                }
                outputs.push_back(o);
            }
            if (outputs.size() % 3 != 0) {
                cout << "Error: number of outputs is not divisible by 3!\n";
            }
            cout << "Blocks drawn: " << blocks << endl;
        } else {
            // Configuration
            bool manual = false;
            bool print_display = false || manual; // Manual mode needs display
            program.writeReg(0,2);
            vector<output_packet> outputs;
            // Initialize displays
            program.sendInput(cJoyNeutral);
            program.setVerbose(false);
            program.run();
            output_packet out(0,0,0);
            while (program.hasOutput()) {
                out.x = program.getOutput();
                out.y = program.getOutput();
                out.c = program.getOutput();
                outputs.push_back(out);
            }
            // Find display bounds
            int64_t score = 0;
            int64_t w = 0, h = 0;
            for (output_packet& o : outputs) {
                if (o.x < 0) {
                    score = o.c;
                } else {
                    w = max(w, o.x);
                    h = max(h, o.y);
                }
            }
            //cout << "Current score: " << score << endl;
            //cout << "Main display size: " << w << "x" << h << endl;
            // Build display
            string line(w+1,cTileChars[0]);
            string frame_line(w+1,'+');
            vector<string> display(h+1,line);
            int paddle_x = 0;
            int ball_x = 0;
            for (output_packet& o : outputs) {
                if (o.x >= 0) {
                    display[o.y][o.x]=cTileChars[o.c];
                    if (o.c == cBall) ball_x = o.x;
                    if (o.c == cPaddle) paddle_x = o.x;
                }
            }
            // Print display
            if (print_display) {
                cout << frame_line << endl;
                cout << "+ SCORE: " << score << endl;
                for (const string &l : display) {
                    cout << l << endl;
                }
            }
            // Play
            bool playing = true;
            while (playing) {
                if (manual) {
                    cout << "Move paddle (asd): \n";
                    char c = getchar();
                    switch (c) {
                        case 'a':
                            program.sendInput(cJoyLeft);
                            break;
                        case 's':
                            program.sendInput(cJoyNeutral);
                            break;
                        case 'd':
                            program.sendInput(cJoyRight);
                            break;
                        default:
                            // skip char
                            break;
                    }
                } else {
                    // Automatic playing
                    if (ball_x > paddle_x) {
                        program.sendInput(cJoyRight);
                    } else if (ball_x == paddle_x) {
                        program.sendInput(cJoyNeutral);
                    } else {
                        program.sendInput(cJoyLeft);
                    }
                }
                program.run();
                if (program.hasTerminated()) playing = false;
                outputs.resize(0);
                while (program.hasOutput()) {
                    out.x = program.getOutput();
                    out.y = program.getOutput();
                    out.c = program.getOutput();
                    outputs.push_back(out);
                }
                for (output_packet& o : outputs) {
                    if (o.x >= 0) {
                        display[o.y][o.x]=cTileChars[o.c];
                        if (o.c == cBall) ball_x = o.x;
                        if (o.c == cPaddle) paddle_x = o.x;
                    }
                    else { score = o.c; }
                }
                if (print_display) {
                    cout << frame_line << endl;
                    cout << "+ SCORE: " << score << endl;
                    for (const string &l : display) {
                        cout << l << endl;
                    }
                }
            }
            cout << "Final score: " << score << endl;
        } // if part_two
    }
}

