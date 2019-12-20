//
// Created by Gyebro on 2019-12-18.
//
#include <algorithm>
#include <bitset>
#include <map>
#include "day18.h"

const size_t paths_n = 400000;
const size_t steps_n = 2500;

typedef bitset<26> keychain;

typedef pair<size_t, size_t> state_type;
state_type get_state_hash(const pair<size_t, size_t> &pos, const keychain &keys, const char &target) {
    size_t p = pos.first + pos.second*100;
    return {static_cast<size_t>(target) +1000*p, keys.to_ullong() };
}

enum maze_tile_type {
    wall,
    open,
    key, // open tile with key
    door, // open with key, wall without key
};

class maze_tile {
public:
    maze_tile_type type;
    char value; // only relevant for key|door
    maze_tile() : value('0'), type(wall) {}
    explicit maze_tile(const char c) : value(c) {
        switch (c) {
            case '#':
                type = wall; break;
            case '.':
            case '@':
                type = open; break;
            default:
                if (c < 'a') { // Note 'A' ... 'Z' < 'a' ... 'z'
                    type = door;
                } else {
                    type = key;
                }
                break;

        }
    }
};

class graph_tile {
public:
    size_t x, y;
    vector<graph_tile*> neighbors;
    bool visited;
    bool wall;
    bool conditional_passage;
    char key_name;
    bool is_key;
    graph_tile() {
        x = y = 0;
        visited = false;
        wall = false;
        is_key = false;
        neighbors.resize(0); // Will be generated later
        conditional_passage = false;
        key_name = '\n';
    }
    graph_tile(size_t X, size_t Y, bool is_wall) : x(X), y(Y), wall(is_wall) {
        visited = false;
        is_key = false;
        neighbors.resize(0); // Will be generated later
        conditional_passage = false;
        key_name = '\n';
    }
};

bool graph_bfs(graph_tile *target_ptr, const size_t max_steps, vector<graph_tile *>& front, size_t& steps, const keychain& keys) {
    // For every element in the frontline nodes, enumerate compatible children
    if (front.size() == 0) return false;
    vector<graph_tile *> new_front;
    graph_tile* t;
    steps++;
    for (size_t i=0; i<front.size(); i++) {
        t = front[i];
        for (graph_tile* n : t->neighbors) {
            if (n->conditional_passage) {
                if (keys.test(n->key_name - 'a')) {
                    // This neighbour is accessible
                } else {
                    continue; // Skip this neighbour as we don't have the key
                }
            }
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
                } else if (n->is_key && !keys.test(n->key_name - 'a')) {
                    // If it's an uncollected key, skip front (other branch will target this key)
                } else {
                    n->visited = true;
                    new_front.push_back(n);
                }
            }
        }
    }
    front = new_front;
    return graph_bfs(target_ptr, max_steps, front, steps, keys);
}

void clear_visited(vector<graph_tile>& graph) {
    for (graph_tile& t : graph) t.visited = false;
}

// Return the distance from start to end taking into account the keys_available
size_t get_distance(vector<graph_tile>& graph, pair<size_t, size_t> start, pair<size_t, size_t> target, const char target_key, const keychain& keys,
                    std::map<state_type, size_t>& storage) {
    state_type s = get_state_hash(start, keys, target_key);
    auto search = storage.find(s);
    if (search != storage.end()) {
        return search->second;
    }
    graph_tile *t_start;
    graph_tile *t_target;
    for (size_t i=0; i<graph.size(); i++) {
        if (graph[i].x == start.first && graph[i].y == start.second) {
            t_start = &(graph[i]);
        }
        if (graph[i].x == target.first && graph[i].y == target.second) {
            t_target = &(graph[i]);
        }
    }
    vector<graph_tile *> front;
    //vector<vector<graph_tile *>> paths;
    //paths.push_back(front); // Empty yet
    front.push_back(t_start);
    //paths.resize(0);
    clear_visited(graph);
    size_t steps = 0;
    if (graph_bfs(t_target, steps_n, front, steps, keys)) {
        // steps contains the actual number of steps
    } else {
        steps = numeric_limits<size_t>::max();
    }
    // Memoization of steps with respect to state
    storage[s]=steps;
    return steps;
}

class hyper_tile {
public:
    pair<size_t, size_t> coord;
    keychain keys;
    size_t steps;
    hyper_tile() : coord({0,0}), keys(0), steps(0) { }
    hyper_tile(const pair<size_t, size_t>& coord,
               const keychain& keys,
            size_t steps) : coord(coord), keys(keys), steps(steps) {}
};

bool hyper_bfs(vector<hyper_tile>& front, size_t& min_steps,
               vector<graph_tile>& graph,
               const vector<vector<maze_tile>>& map,
               const vector<pair<pair<size_t, size_t>, char>>& keys,
               std::map<state_type, size_t>& storage) {;
    bool debug = true;
    if (front.size() == 0) return false;
    vector<hyper_tile> new_front;
    size_t uncollected = 0;
    size_t steps_treshold = numeric_limits<size_t>::max();
    if (debug) cout << "Search front: " << front.size() << " keys: " << front[0].keys.count() << "/" << keys.size() << endl;
    if (front.size() > paths_n) {
        vector<size_t> st;
        for (const hyper_tile& t : front) {
            st.push_back(t.steps);
        }
        sort(st.begin(), st.end());
        steps_treshold = st[paths_n];
        if (debug) cout << "Dropping path if accumulated steps > " << steps_treshold << endl;
    }
    for (const hyper_tile& t : front) {
        if (t.steps > steps_treshold) {
            continue;
        }
        // Generate neighbours towards uncollected keys
        uncollected = 0;
        for (const pair<pair<size_t, size_t>, char>& key : keys) {
            if (!t.keys.test(key.second-'a')) {
                // Key is still uncollected
                uncollected++;
                size_t extra_steps = get_distance(graph, t.coord, key.first, key.second, t.keys, storage);
                if (extra_steps < numeric_limits<size_t>::max()) {
                    // Key is accessible from t
                    keychain key_list = t.keys;
                    key_list[key.second-'a']=true;
                    new_front.emplace_back(hyper_tile(key.first, key_list, t.steps+extra_steps));
                }
            } else {
                // Key already collected
            }
        }
        // Check if we have all keys already
        if (uncollected == 0) {
            /*if (debug) {
                cout << "Pos: (" << t.coord.first << "," << t.coord.second << "), keys: [";
                for (const char kk : t.keys) cout << kk;
                cout << "], steps: " << t.steps << endl;
            }*/
            if (t.steps < min_steps) {
                min_steps = t.steps;
            }
        }
    }
    front = new_front;
    return hyper_bfs(front, min_steps, graph, map, keys, storage);
}

void build_graph_from_map(const vector<vector<maze_tile>>& map, vector<graph_tile>& graph) {
    size_t x=0, y=0;
    // Build graph
    size_t W = map[0].size();
    size_t H = map.size();
    graph.resize(0);
    graph.reserve(W*H);
    for (const vector<maze_tile>& lines : map) {
        x = 0;
        for (const maze_tile& t : lines) {
            graph_tile tl;
            tl.x = x; tl.y = y;
            tl.conditional_passage = false;
            tl.key_name = '0';
            tl.is_key = false;
            switch (t.type) {
                case wall:
                    tl.wall = true;
                    break;
                case open:
                    tl.wall = false;
                    break;
                case key:
                    tl.wall = false;
                    tl.is_key = true;
                    tl.key_name = t.value;
                    break;
                case door:
                    char key_needed = t.value + ('a'-'A');
                    tl.wall = false;
                    tl.conditional_passage = true;
                    tl.key_name = key_needed;
                    break;
            }
            graph.push_back(tl);
            x++;
        }
        y++;
    }
    // Build edges
    for (graph_tile& t : graph) {
        if (!t.wall) {
            // Check four neighbours in reading order
            x = t.x; y = t.y;
            if (y > 0) {   if (!graph[(y-1)*W+x].wall) t.neighbors.push_back(&graph[(y-1)*W+x]); } // Top neighbour
            if (x > 0) {   if (!graph[y*W+(x-1)].wall) t.neighbors.push_back(&graph[y*W+(x-1)]); } // Left
            if (x < W-1) { if (!graph[y*W+(x+1)].wall) t.neighbors.push_back(&graph[y*W+(x+1)]); } // Right
            if (y < H-1) { if (!graph[(y+1)*W+x].wall) t.neighbors.push_back(&graph[(y+1)*W+x]); } // Bottom
        }
        // NOTE: Doors are considered as open path with conditional_passage = true
    }
}

size_t collect_keys(const vector<vector<maze_tile>>& map, const vector<pair<pair<size_t, size_t>, char>>& keys, pair<size_t, size_t> start) {
    // Setup graph for key-distance BFS searches
    vector<graph_tile> graph;
    build_graph_from_map(map, graph);
    hyper_tile t;
    t.coord = start;
    t.keys = keychain(0);
    t.steps = 0;
    vector<hyper_tile> front;
    front.push_back(t);
    size_t min_steps = numeric_limits<size_t>::max();
    std::map<state_type, size_t> distance_storage;
    hyper_bfs(front, min_steps, graph, map, keys, distance_storage);
    return min_steps;
}

void day18(bool part_two) {
    cout << "AoC D18: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input18.txt");
    vector<vector<maze_tile>> map;
    if (in.is_open()) {
        string line;
        vector<maze_tile> map_line;
        size_t x=0, y=0;
        pair<size_t, size_t> entrance;
        vector<pair<pair<size_t, size_t>, char>> keys;
        vector<pair<pair<size_t, size_t>, char>> doors;
        while (getline(in, line)) {
            map_line.resize(0);
            x=0;
            for (const char& c : line) {
                switch (c) {
                    case '@':
                        entrance = {x, y};
                        break;
                    case '#':
                    case '.':
                        // Walls and open space, nothing to do
                        break;
                    default:
                        if (c < 'a') { // Note 'A' ... 'Z' < 'a' ... 'z'
                            doors.push_back({{x, y},c});
                        } else {
                            keys.push_back({{x, y},c});
                        }
                        break;
                }
                map_line.emplace_back(maze_tile(c));
                x++;
            }
            map.push_back(map_line);
            y++;
        }
        if (!part_two) {
            cout << "Found " << keys.size() << " keys\n";
            cout << "Found " << doors.size() << " doors\n";
            cout << "Entrance is at: " << entrance.first << "," << entrance.second << endl;
            // Original plan: Build a graph starting from entrance
            // For every node: edges towards uncollected and accessible keys are weighted by steps needed
            size_t steps = collect_keys(map, keys, entrance);
            if (steps < numeric_limits<size_t>::max()) {
                cout << "Steps to collect keys: " << steps << endl;
            } else {
                cout << "Failed to collect all keys, modify search heuristics!\n";
            }
        }


    }
}
