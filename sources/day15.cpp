//
// Created by Gyebro on 2019-12-17.
//
#include "day15.h"
#include "intcode_vm.h"
#include <stack>

const int64_t repair_droid_north = 1;
const int64_t repair_droid_south = 2;
const int64_t repair_droid_west = 3;
const int64_t repair_droid_east = 4;

const int64_t repair_droid_status_blocked = 0;
const int64_t repair_droid_status_moved = 1;
const int64_t repair_droid_status_target_reached = 2;

enum class tile_type {
    unknown,
    wall,
    path
};

class oxygen_system_tile {
public:
    int x;
    int y;
    bool visited;
    bool goal;
    tile_type type;
    oxygen_system_tile(int x, int y) : x(x), y(y) {
        type = tile_type::unknown;
        goal = false;
        visited = false;
    }
};

// Returns true if neighbour is accessible
bool discover_neighbour(IntcodeProgram& droid, vector<vector<oxygen_system_tile>>& map, pair<int,int>& loc, int64_t move_cmd) {
    switch (move_cmd) {
        case repair_droid_north:
            loc.second++;
            break;
        case repair_droid_south:
            loc.second--;
            break;
        case repair_droid_east:
            loc.first++;
            break;
        case repair_droid_west:
            loc.first--;
            break;
        default:
            cout << "Invalid input!\n";
            return false;
    }
    if (!map[loc.second][loc.first].visited) {
        droid.sendInput(move_cmd);
        droid.run();
        if (droid.hasOutput()) {
            switch (droid.getOutput()) {
                case repair_droid_status_blocked:
                    map[loc.second][loc.first].type=tile_type::wall;
                    map[loc.second][loc.first].visited=true;
                    return false;
                    break;
                case repair_droid_status_moved:
                    map[loc.second][loc.first].type=tile_type::path;
                    break;
                case repair_droid_status_target_reached:
                    map[loc.second][loc.first].type=tile_type::path;
                    map[loc.second][loc.first].goal=true;
                    break;
            }
        } else {
            cout << "Droid not responding!\n";
            return false;
        }
    } else {
        return false; // Already visited
    }
    return true;
}

bool oxygen_system_bfs(vector<vector<oxygen_system_tile>>& map, pair<int,int> target, vector<pair<int,int>>& front, size_t& steps) {
    // For every element in the frontline nodes, enumerate compatible children
    if (front.size() == 0) return false;
    steps++;
    vector<pair<int,int>> new_front;
    for (pair<int,int> c : front) {
        vector<pair<int,int>> neighbors;
        if (map[c.second+1][c.first].type==tile_type::path) neighbors.push_back(pair<int,int>(c.first,c.second+1));
        if (map[c.second-1][c.first].type==tile_type::path) neighbors.push_back(pair<int,int>(c.first,c.second-1));
        if (map[c.second][c.first+1].type==tile_type::path) neighbors.push_back(pair<int,int>(c.first+1,c.second));
        if (map[c.second][c.first-1].type==tile_type::path) neighbors.push_back(pair<int,int>(c.first-1,c.second));
        for (const pair<int,int>& n : neighbors) {
            if (n == target) {
                // Target reached
                new_front.clear();
                front = new_front;
                return true;
            } else {
                if (map[n.second][n.first].visited)  {
                    // skip, already visited
                } else {
                    map[n.second][n.first].visited = true;
                    new_front.push_back(n);
                }
            }
        }
    }
    front = new_front;
    return oxygen_system_bfs(map, target, front, steps);
}

void day15(bool part_two) {
    cout << "AoC D15: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input15.txt");
    IntcodeProgram droid(in);
    droid.reserveMemory(5000);
    bool searching = true;
    size_t map_size = 500;
    int x = map_size/2;
    int y = map_size/2;
    stack<pair<pair<int, int>, IntcodeProgram>> S; // Stack for current location and a copy of droid's program
    vector<vector<oxygen_system_tile>> map;
    for (size_t Y=0; Y<map_size; Y++) {
        vector<oxygen_system_tile> line;
        for (size_t X=0; X<map_size; X++) {
            line.emplace_back(oxygen_system_tile(X,Y));
        }
        map.push_back(line);
    }
    map[y][x].type=tile_type::path;
    pair<int,int> loc = pair<int,int>(x,y);
    pair<int,int> start, goal;
    start=loc;
    S.push({loc, droid});
    IntcodeProgram droid_copy=droid;
    pair<int,int> loc_copy;
    while (searching) {
        // DFS from top of stack
        if (S.empty()) {
            cout << "Entire oxygen system discovered!\n";
            break;
        }
        loc=S.top().first;
        droid=S.top().second;
        //cout << "Droid is at: " << loc.first << "," << loc.second << endl;
        S.pop();
        // We are at vertex now
        map[loc.second][loc.first].visited = true;
        if (map[loc.second][loc.first].goal) {
            //cout << "Goal found "<< loc.first << "," << loc.second << "\n";
            goal = loc;
            if (!part_two) searching = false;
        }
        // Discover neighbours if needed
        droid_copy = droid; loc_copy = loc;
        if (discover_neighbour(droid_copy, map, loc_copy, repair_droid_north)) {
            //cout << "Adding target tile: " << loc_copy.first << "," << loc_copy.second << endl;
            S.push({loc_copy,droid_copy});
        }
        droid_copy = droid; loc_copy = loc;
        if (discover_neighbour(droid_copy, map, loc_copy, repair_droid_south)) {
            //cout << "Adding target tile: " << loc_copy.first << "," << loc_copy.second << endl;
            S.push({loc_copy,droid_copy});
        }
        droid_copy = droid; loc_copy = loc;
        if (discover_neighbour(droid_copy, map, loc_copy, repair_droid_east)) {
            //cout << "Adding target tile: " << loc_copy.first << "," << loc_copy.second << endl;
            S.push({loc_copy,droid_copy});
        }
        droid_copy = droid; loc_copy = loc;
        if (discover_neighbour(droid_copy, map, loc_copy, repair_droid_west)) {
            //cout << "Adding target tile: " << loc_copy.first << "," << loc_copy.second << endl;
            S.push({loc_copy,droid_copy});
        }
    } // searching
    if (!part_two) {
        for (vector<oxygen_system_tile> &line : map) {
            for (oxygen_system_tile &tile : line) {
                tile.visited = false;
            }
        }
        vector<pair<int, int>> search;
        search.push_back(start);
        size_t steps = 0;
        oxygen_system_bfs(map, goal, search, steps);
        cout << "Steps to reach oxygen system: " << steps << endl;
    } else {
        for (vector<oxygen_system_tile> &line : map) {
            for (oxygen_system_tile &tile : line) {
                tile.visited = false;
            }
        }
        vector<pair<int, int>> search;
        search.push_back(goal);
        size_t steps = 0;
        pair<int,int> unreachable_target = {1,1};
        oxygen_system_bfs(map, unreachable_target, search, steps);
        cout << "Minutes to fill the entire oxygen system: " << steps-1 << endl; // Last step of neighbour check subtracted
    }
}

