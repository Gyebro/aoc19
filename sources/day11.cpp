//
// Created by Gyebro on 2019-12-11.
//
#include "day11.h"
#include "intcode_vm.h"

typedef pair<int, int> vec2i;
struct output_packet {
    vec2i coord;
    bool black;
};

const size_t cRight = 0;
const size_t cUp = 1;
const size_t cLeft = 2;
const size_t cDown = 3;

void day11(bool part_two) {
    cout << "AoC D11: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input11.txt");
    if (in.is_open()) {
        IntcodeProgram program(in);
        program.reserveMemory(1500);
        vector<output_packet> tiles; // The robot's path
        vec2i pos = {0,0};
        size_t idx;
        bool been_there = false;
        int dir = 1;
        bool done = false;
        size_t newly_painted = 0;
        if (part_two) {
            tiles.push_back({pos,false}); // Start with a white tile
        }
        while (!program.hasTerminated() && !done) {
            // Get color of current pos
            idx = find_idx_if(tiles, [pos](const output_packet &t) { return t.coord == pos; });
            if (idx == tiles.size()) {
                // Create a new tile (black initially), idx will be a valid index now
                tiles.push_back({pos,true});
                been_there = false;
            } else {
                been_there = true;
            }
            int64_t color = tiles[idx].black ? 0 : 1;
            // Send input
            program.sendInput(color);
            // Run
            program.run();
            // Get outputs
            if (program.hasOutput()) {
                int64_t new_color = program.getOutput();
                //cout << "Robot paints (" << pos.first << "," << pos.second << ") to " << new_color << endl;
                if (!been_there) newly_painted++;
                // Paint the current tile
                tiles[idx].black = (new_color == 0);
                if (program.hasOutput()) {
                    int64_t turn = program.getOutput();
                    switch (turn) {
                        case 0: // left
                            dir++;
                            dir = dir % 4;
                            break;
                        case 1: // right
                            dir--;
                            if (dir<0) dir+=4;
                            break;
                        default:
                            cout << "Invalid turn output\n";
                            break;
                    }
                    // Move
                    switch (dir) {
                        case cUp:
                            pos.second++;
                            break;
                        case cLeft:
                            pos.first--;
                            break;
                        case cDown:
                            pos.second--;
                            break;
                        case cRight:
                            pos.first++;
                            break;
                        default:
                            cout << "Invalid direction\n";
                            break;
                    }
                } else {
                    done = true;
                }
            } else {
                done = true;
            } // if (has output)
        } // while
        if (!part_two) {
            cout << "Newly painted tiles: " << newly_painted << endl;
        } else {
            // Print hull
            int xmin = 0;
            int xmax = 0;
            int ymin = 0;
            int ymax = 0;
            for (const output_packet& t : tiles) {
                xmin = min(xmin, t.coord.first);
                xmax = max(xmax, t.coord.first);
                ymin = min(ymin, t.coord.second);
                ymax = max(ymax, t.coord.second);
            }
            cout << "Painted hull area is within (" << xmin << "," << ymin << ") and " <<
                                  "(" << xmax << "," << ymax << ")\n";
            vector<char> line(xmax-xmin+1,' ');
            vector<vector<char>> licence_plate(ymax-ymin+1, line);
            for (const output_packet& t : tiles) {
                size_t x = t.coord.first  - xmin;
                size_t y = t.coord.second - ymin;
                licence_plate[y][x] = t.black ? ' ' : '#';
            }
            for (auto it = licence_plate.rbegin(); it != licence_plate.rend(); ++it) {
                for (char& c : *it) {
                    cout << c;
                }
                cout << endl;
            }
        }
    }
}