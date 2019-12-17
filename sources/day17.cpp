//
// Created by Gyebro on 2019-12-17.
//
#include "day17.h"
#include "intcode_vm.h"

enum class robot_dir {
    Up,
    Left,
    Down,
    Right
};

vector<string> get_camera_feed(IntcodeProgram& program) {
    vector<string> image;
    string line = "";
    char o;
    while (program.hasOutput()) {
        if (program.readOutput() == 'M' || program.readOutput() > 255) {
            // Program asking for 'Main' routine
            break;
        }
        o = static_cast<char>(program.getOutput());
        switch (o) {
            case '\n':
                image.push_back(line);
                line="";
                break;
            default:
                line+=o;
                break;
        }
    }
    return image;
}

void day17(bool part_two) {
    cout << "AoC D17: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input17.txt");
    // Configuration
    bool print_map = false; // part one
    bool interactive = false; // part two
    if (in.is_open()) {
        IntcodeProgram program(in);
        program.reserveMemory(5000);
        program.run();
        vector<string> map = get_camera_feed(program);
        size_t w = map[0].size();
        size_t h = map.size();
        if (!part_two) {
            // Part one: calculate alignment checksum
            size_t sum = 0;
            for (size_t i = 1; i < w - 1; i++) {
                for (size_t j = 1; j < h - 1; j++) {
                    if (map[j][i] == '#') {
                        if ((map[j - 1][i] == '#') &&
                            (map[j + 1][i] == '#') &&
                            (map[j][i - 1] == '#') &&
                            (map[j][i + 1] == '#')) {
                            // Note: Intersections on the edge are not supported
                            map[j][i] = 'O';
                            sum += i * j;
                        }
                    }
                }
            }
            cout << "Camera alignment sum: " << sum << endl;
            if (print_map) {
                for (const string& l : map) {
                    cout << l << endl;
                }
            }
        } else {
            // Part Two
            // Plan route according to first execution
            pair<int, int> pos;
            robot_dir dir;
            for (size_t i = 0; i < w ; i++) {
                for (size_t j = 0; j < h; j++) {
                    switch (map[j][i]) {
                        case '^':
                            dir = robot_dir::Up;
                            pos = {i,j};
                            break;
                        case 'v':
                            dir = robot_dir::Down;
                            pos = {i,j};
                            break;
                        case '<':
                            dir = robot_dir::Left;
                            pos = {i,j};
                            break;
                        case '>':
                            dir = robot_dir::Right;
                            pos = {i,j};
                            break;
                    }
                }
            }
            cout << "Vacuum robot is at: " << pos.first << "," << pos.second << endl;
            // Change program at address 0 to value '2'
            in.close();
            in.open("input17.txt");
            IntcodeProgram p(in);
            p.reserveMemory(10000000);
            p.writeReg(0,2);
            p.run();
            bool running = true;
            while (running) {
                string line;
                if (p.hasTerminated()) {
                    //cout << "Program terminated\n";
                    running = false;
                }
                if (p.hasOutput()) {
                    if (p.readLastOutput() > 255) {
                        // Non-ascii character:
                        cout << "Program returned: " << p.readLastOutput() << endl;
                        running = false;
                    } else if (p.readOutput() == 'M') {
                        // Dump the rest of the output
                        while (p.hasOutput()) {
                            cout << static_cast<char>(p.getOutput());
                        }
                        cout << endl;
                    } else {
                        // Display camera feed
                        cout << "Displaying camera feed:\n";
                        vector<string> image = get_camera_feed(p);
                        if (print_map) { for (const string &l : image) { cout << l << endl; } }
                    }
                } else {
                    string routine, funA, funB, funC, video;
                    // Main routine
                    if (interactive) {
                        getline(cin,routine); routine+='\n';
                    } else {
                        routine = "A,B,A,B,C,C,B,A,C,A\n";
                        cout << routine;
                    }
                    p.sendInputString(routine); p.run();
                    while(p.hasOutput()) {cout << static_cast<char>(p.getOutput());}
                    // Fun A
                    if (interactive) {
                        getline(cin,funA); funA+='\n';
                    } else {
                        funA = "L,10,R,8,R,6,R,10\n";
                        cout << funA;
                    }
                    p.sendInputString(funA); p.run();
                    while(p.hasOutput()) {cout << static_cast<char>(p.getOutput());}
                    // Fun B
                    if (interactive) {
                        getline(cin, funB); funB += '\n';
                    } else {
                        funB = "L,12,R,8,L,12\n";
                        cout << funB;
                    }
                    p.sendInputString(funB); p.run();
                    while(p.hasOutput()) {cout << static_cast<char>(p.getOutput());}
                    // Fun C
                    if (interactive) {
                        getline(cin,funC); funC+='\n';
                    } else {
                        funC = "L,10,R,8,R,8\n";
                        cout << funC;
                    }
                    p.sendInputString(funC); p.run();
                    while(p.hasOutput()) {cout << static_cast<char>(p.getOutput());}
                    // Feed
                    if (interactive) {
                        getline(cin, video); video+='\n';
                    } else {
                        video = "n\n";
                        cout << video;
                    }
                    p.sendInputString(video); p.run();
                }
            }
        }
    }
}