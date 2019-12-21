//
// Created by Gyebro on 2019-12-20.
//
#include "day20.h"

class donut_tile {
public:
    size_t x, y, z;
    vector<donut_tile*> neighbors;
    bool visited;
    bool wall;
    bool portal;
    bool outer;
    string portal_name;
    donut_tile() : visited(false), wall(false), portal(false), outer(false) {
        x = y = z = 0;
        neighbors.resize(0); // Will be generated later
    }
    donut_tile(size_t X, size_t Y) : x(X), y(Y), z(0), visited(false), wall(false), portal(false), outer(false) {
        neighbors.resize(0); // Will be generated later
    }
    donut_tile(size_t X, size_t Y, size_t Z) : x(X), y(Y), z(Z), visited(false), wall(false), portal(false), outer(false) {
        neighbors.resize(0); // Will be generated later
    }
};

bool is_capital_letter(char c) {
    return ('A' <= c) && (c <= 'Z');
}

bool donut_bfs(donut_tile *target_ptr, const size_t max_steps, vector<donut_tile *>& front, size_t& steps) {
    // For every element in the frontline nodes, enumerate compatible children
    if (front.size() == 0) return false;
    vector<donut_tile *> new_front;
    donut_tile* t;
    steps++;
    for (size_t i=0; i<front.size(); i++) {
        t = front[i];
        for (donut_tile* n : t->neighbors) {
            if (n == target_ptr) {
                // Target reached
                // Update paths with this single one
                new_front.clear();
                front = new_front;
                return true;
            } else {
                if (n->visited)  {
                    // skip, already visited
                } else if (steps > max_steps) {
                    // Skip this node, max step constraint
                } else {
                    n->visited = true;
                    new_front.push_back(n);
                }
            }
        }
    }
    front = new_front;
    return donut_bfs(target_ptr, max_steps, front, steps);
}

void day20(bool part_two) {
    cout << "AoC D20: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input20.txt");
    vector<string> map_str;
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            map_str.push_back(line);
        }
        size_t W = map_str[0].size();
        size_t H = map_str.size();
        if (!part_two) { // Part one
            vector<donut_tile> donut;
            donut.reserve(W * H);
            string portal_name;
            size_t x, y;
            for (y = 0; y < H; y++) {
                for (x = 0; x < W; x++) {
                    donut_tile t(x, y);
                    switch (map_str[y][x]) {
                        case '.':
                            if (is_capital_letter(map_str[y - 1][x])) {
                                // Label from top
                                portal_name = string(1, map_str[y - 2][x]) + string(1, map_str[y - 1][x]);
                            } else if (is_capital_letter(map_str[y + 1][x])) {
                                // Label from below
                                portal_name = string(1, map_str[y + 1][x]) + string(1, map_str[y + 2][x]);
                            } else if (is_capital_letter(map_str[y][x - 1])) {
                                // Label from left
                                portal_name = string(1, map_str[y][x - 2]) + string(1, map_str[y][x - 1]);
                            } else if (is_capital_letter(map_str[y][x + 1])) {
                                // Label from right
                                portal_name = string(1, map_str[y][x + 1]) + string(1, map_str[y][x + 2]);
                            } else {
                                portal_name = "";
                            }
                            t.portal = !(portal_name == "");
                            t.portal_name = portal_name;
                            t.wall = false;
                            break;
                        case '#':
                            t.wall = true;
                            break;
                        default: // Letters and spaces
                            t.wall = true;
                            break;
                    }
                    donut.push_back(t);
                }
            }
            donut_tile *start;
            donut_tile *end;
            // Build edges
            for (donut_tile &t : donut) {
                if (!t.wall) {
                    // Check four neighbours in reading order
                    x = t.x;
                    y = t.y;
                    if (y > 0) {
                        if (!donut[(y - 1) * W + x].wall)
                            t.neighbors.push_back(&donut[(y - 1) * W + x]);
                    } // Top neighbour
                    if (x > 0) {
                        if (!donut[y * W + (x - 1)].wall)
                            t.neighbors.push_back(&donut[y * W + (x - 1)]);
                    } // Left
                    if (x < W - 1) {
                        if (!donut[y * W + (x + 1)].wall)
                            t.neighbors.push_back(&donut[y * W + (x + 1)]);
                    } // Right
                    if (y < H - 1) {
                        if (!donut[(y + 1) * W + x].wall)
                            t.neighbors.push_back(&donut[(y + 1) * W + x]);
                    } // Bottom
                    // add portal pair if needed
                    if (t.portal) {
                        bool pair_found = false;
                        for (size_t z = 0; z < donut.size(); z++) {
                            donut_tile o = donut[z];
                            if (o.portal && o.portal_name == t.portal_name && (o.x != x) && (o.y != y)) {
                                // Portal pair found
                                t.neighbors.push_back(&donut[z]);
                                pair_found = true;
                            }
                        }
                        if (!pair_found) {
                            if (t.portal_name == "AA") {
                                start = &t;
                            } else if (t.portal_name == "ZZ") {
                                end = &t;
                            } else {
                                cout << "Warning: portal '" << t.portal_name << "' doesn't have a pair!\n";
                            }
                        }
                    }
                }
            } // for
            vector<donut_tile *> front;
            front.push_back(start);
            size_t steps = 0;
            donut_bfs(end, 1000, front, steps);
            cout << "Steps to exit the donut: " << steps << endl;
        } else {
            // Part two
            vector<donut_tile> donut;
            size_t D = 40; // Depth
            donut.reserve(W * H * D);
            string portal_name;
            bool outer = false;
            size_t x, y, z;
            for (z = 0; z < D; z++) {
                for (y = 0; y < H; y++) {
                    for (x = 0; x < W; x++) {
                        donut_tile t(x, y, z);
                        switch (map_str[y][x]) {
                            case '.':
                                outer = (x < 4 || x > W-4) || (y < 4 || y > H-4);
                                if (is_capital_letter(map_str[y - 1][x])) {
                                    // Label from top
                                    portal_name = string(1, map_str[y - 2][x]) + string(1, map_str[y - 1][x]);
                                } else if (is_capital_letter(map_str[y + 1][x])) {
                                    // Label from below
                                    portal_name = string(1, map_str[y + 1][x]) + string(1, map_str[y + 2][x]);
                                } else if (is_capital_letter(map_str[y][x - 1])) {
                                    // Label from left
                                    portal_name = string(1, map_str[y][x - 2]) + string(1, map_str[y][x - 1]);
                                } else if (is_capital_letter(map_str[y][x + 1])) {
                                    // Label from right
                                    portal_name = string(1, map_str[y][x + 1]) + string(1, map_str[y][x + 2]);
                                } else {
                                    portal_name = "";
                                }
                                t.portal = !(portal_name == "");
                                t.outer = outer;
                                t.portal_name = portal_name;
                                t.wall = false;
                                break;
                            case '#':
                                t.wall = true;
                                break;
                            default: // Letters and spaces
                                t.wall = true;
                                break;
                        }
                        donut.push_back(t);
                    }
                }
            }
            donut_tile *start;
            donut_tile *end;
            // Build edges
            for (donut_tile &t : donut) {
                if (!t.wall) {
                    // Check four neighbours in reading order
                    x = t.x; y = t.y; z = t.z;
                    if (y > 0) {
                        if (!donut[z*W*H + (y - 1) * W + x].wall)
                            t.neighbors.push_back(&donut[z*W*H + (y - 1) * W + x]);
                    } // Top neighbour
                    if (x > 0) {
                        if (!donut[z*W*H + y * W + (x - 1)].wall)
                            t.neighbors.push_back(&donut[z*W*H + y * W + (x - 1)]);
                    } // Left
                    if (x < W - 1) {
                        if (!donut[z*W*H + y * W + (x + 1)].wall)
                            t.neighbors.push_back(&donut[z*W*H + y * W + (x + 1)]);
                    } // Right
                    if (y < H - 1) {
                        if (!donut[z*W*H + (y + 1) * W + x].wall)
                            t.neighbors.push_back(&donut[z*W*H + (y + 1) * W + x]);
                    } // Bottom
                    if (t.portal) {
                        bool pair_found = false;
                        for (size_t p = 0; p < W*H; p++) {
                            donut_tile o = donut[p];
                            if (o.portal && o.portal_name == t.portal_name && (o.x != x) && (o.y != y)) {
                                // Portal pair found
                                if (t.outer) { // outer portal: outer
                                    if (z > 0) {
                                        t.neighbors.push_back(&donut[(z-1)*W*H + p]);
                                    } else {
                                        // Can't go outer than the outermost layer
                                    }
                                } else { // inner portal: deeper
                                    if (z < D-1) {
                                        t.neighbors.push_back(&donut[(z+1)*W*H + p]);
                                    }
                                }
                                pair_found = true;
                            }
                        }
                        if (!pair_found && z == 0) {
                            if (t.portal_name == "AA") {
                                start = &t;
                            } else if (t.portal_name == "ZZ") {
                                end = &t;
                            }
                        }
                    }
                }
            } // for building edges
            cout << "Recursive donut maze size: " << donut.size() << endl;
            vector<donut_tile *> front;
            front.push_back(start);
            size_t steps = 0;
            donut_bfs(end, 10000, front, steps);
            cout << "Steps to exit the recursive donut: " << steps << endl;
        }
    }
}
