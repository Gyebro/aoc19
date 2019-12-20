//
// Created by Gyebro on 2019-12-18.
//
#include <algorithm>
#include "day18.h"

const size_t paths_n = 50000;
const size_t steps_n = 500;

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
    bool wall;
    bool conditional_passage;
    char conditional_key;
    graph_tile() {
        x = y = 0;
        wall = false;
        neighbors.resize(0); // Will be generated later
        conditional_passage = false;
        conditional_key = '\n';
    }
    graph_tile(size_t X, size_t Y, bool is_wall) : x(X), y(Y), wall(is_wall) {
        neighbors.resize(0); // Will be generated later
        conditional_passage = false;
        conditional_key = '\n';
    }
};

bool graph_bfs(graph_tile const* target_ptr, const size_t max_steps, vector<graph_tile const *>& front, vector<vector<graph_tile const *>>& paths, const vector<char>& keys) {
    // For every element in the frontline nodes, enumerate compatible children
    if (front.size() == 0) return false;
    //cout << "BFS: frontline nodes: " << front.size() << endl;
    vector<graph_tile const*> new_front;
    vector<vector<graph_tile const*>> new_paths;
    graph_tile const* t;
    for (size_t i=0; i<front.size(); i++) {
        t = front[i];
        for (graph_tile const* n : t->neighbors) {
            if (n->conditional_passage) {
                if (contains(keys, n->conditional_key)) {
                    // This neighbour is accessible
                } else {
                    continue; // Skip this neighbour as we don't have the key
                }
            }
            if (n == target_ptr) {
                // Target reached
                // Update paths with this single one
                new_front.clear();
                new_paths.clear();
                front = new_front;
                new_paths.push_back(paths[i]);
                new_paths.back().push_back(n);
                paths = new_paths;
                return true;
            } else {
                // Check for max steps and already visited
                if (paths[i].size()+1 >= max_steps) {
                    // Skip this node, max step constraint
                } else if (contains(paths, n)) {
                    // Skip this node as current path already visited n
                } else if (contains(new_front, n)) {
                    // Skip this node as it is already enumerated in the current depth
                } else {
                    new_front.push_back(n);
                    new_paths.push_back(paths[i]);
                    new_paths.back().push_back(n);
                }
            }
        }
    }
    front = new_front;
    paths = new_paths;
    return graph_bfs(target_ptr, max_steps, front, paths, keys);
}

// Return the distance from start to end taking into account the keys_available
size_t get_distance(const vector<graph_tile>& graph, pair<size_t, size_t> start, pair<size_t, size_t> target, const vector<char>& keys) {
    graph_tile const *t_start;
    graph_tile const *t_target;
    for (size_t i=0; i<graph.size(); i++) {
        if (graph[i].x == start.first && graph[i].y == start.second) {
            t_start = &(graph[i]);
        }
        if (graph[i].x == target.first && graph[i].y == target.second) {
            t_target = &(graph[i]);
        }
    }
    vector<graph_tile const *> front;
    vector<vector<graph_tile const *>> paths;
    paths.push_back(front); // Empty yet
    front.push_back(t_start);
    paths.resize(0);
    if (graph_bfs(t_target, steps_n, front, paths, keys)) {
        return paths[0].size();
    } else {
        return numeric_limits<size_t>::max();
    }
}

class hyper_tile {
public:
    pair<size_t, size_t> coord;
    vector<char> keys;
    size_t steps;
    hyper_tile() : coord({0,0}), keys(0), steps(0) { }
    hyper_tile(const pair<size_t, size_t>& coord,
            const vector<char>& keys,
            size_t steps) : coord(coord), keys(keys), steps(steps) {}
};

bool hyper_bfs(vector<hyper_tile>& front, pair<size_t, string>& min_steps,
               const vector<graph_tile>& graph,
               const vector<vector<maze_tile>>& map,
               const vector<pair<pair<size_t, size_t>, char>>& keys) {
    bool debug = true;
    if (front.size() == 0) return false;
    vector<hyper_tile> new_front;
    size_t uncollected = 0;
    size_t steps_treshold = numeric_limits<size_t>::max();
    if (debug) cout << "Search front: " << front.size() << " keys: " << front[0].keys.size() << "/" << keys.size() << endl;
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
            if (!contains(t.keys, key.second)) {
                // Key is still uncollected
                uncollected++;
                size_t extra_steps = get_distance(graph, t.coord, key.first, t.keys);
                if (extra_steps < numeric_limits<size_t>::max()) {
                    // Key is accessible from t
                    vector<char> key_list = t.keys;
                    key_list.push_back(key.second);
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
            if (t.steps < min_steps.first) {
                min_steps.first = t.steps;
                string keys_str;
                for (const char& cc : t.keys) keys_str+=cc;
                min_steps.second = keys_str;
            }
        }
    }
    front = new_front;
    return hyper_bfs(front, min_steps, graph, map, keys);
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
            tl.conditional_key = '0';
            switch (t.type) {
                case wall:
                    tl.wall = true;
                    break;
                case open:
                case key:
                    tl.wall = false;
                    break;
                case door:
                    char key_needed = t.value + ('a'-'A');
                    tl.wall = false;
                    tl.conditional_passage = true;
                    tl.conditional_key = key_needed;
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
    t.keys.resize(0);
    t.steps = 0;
    vector<hyper_tile> front;
    front.push_back(t);
    pair<size_t,string> min_steps;
    min_steps.first = numeric_limits<size_t>::max();
    min_steps.second = "";
    hyper_bfs(front, min_steps, graph, map, keys);
    cout << "Shortest path: " << min_steps.second << endl;
    return min_steps.first;
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
        cout << "Found " << keys.size() << " keys\n";
        cout << "Found " << doors.size() << " doors\n";
        cout << "Entrance is at: " << entrance.first << "," << entrance.second << endl;
        // Original plan: Build a graph starting from entrance
        // For every node: edges towards uncollected and accessible keys are weighted by steps needed
        size_t steps = collect_keys(map, keys, entrance);
        cout << "Steps to collect keys: " << steps << endl;
    }
}
