//
// Created by Gyebro on 2019-12-05.
//
#include "day03.h"

typedef pair<int, int> vec2i;

class WireSegment {
private:
    vec2i start;
    vec2i end;
    int umin, umax;
    int v;
    size_t arclen;
    bool horizontal;
public:
    WireSegment(vec2i coord, string keyword, size_t arc_length) {
        start = coord;
        arclen = arc_length;
        char c = keyword[0];
        int num = stoi(keyword.substr(1));
        switch (c) {
            case 'L':
                coord.first-=num; horizontal=true;
                break;
            case 'R':
                coord.first+=num; horizontal=true;
                break;
            case 'D':
                coord.second-=num; horizontal=false;
                break;
            case 'U':
                coord.second+=num; horizontal=false;
                break;
            default:
                cout << "Error: invalid wire segment keyword!\n";
                break;
        }
        end = coord;
        if (horizontal) {
            v = start.second;
            umin = min(start.first, end.first);
            umax = max(start.first, end.first);
        } else {
            v = start.first;
            umin = min(start.second, end.second);
            umax = max(start.second, end.second);
        }
    }
    const vec2i& getEnd() const {
        return end;
    }
    size_t getArcLength(const vec2i& pos) const {
        if (horizontal) {
            if (v == pos.second) {
                return arclen + abs(pos.first - start.first);
            } else {
                return 0;
            }
        } else {
            if (v == pos.first) {
                return arclen + abs(pos.second - start.second);
            } else {
                return 0;
            }
        }
    }
    bool isHorizontal() const {
        return horizontal;
    }
    const int& getValue() const {
        return v;
    }
    const int& getMin() const {
        return umin;
    }
    const int& getMax() const {
        return umax;
    }
    bool intersectsWith(const WireSegment& other, vec2i& intersection) const {
        bool intersects = false;
        if (horizontal != other.isHorizontal()) {
            if ((other.getMin() <= v) && (v <= other.getMax())) {
                if ((umin <= other.getValue()) && (other.getValue() <= umax)) {
                    intersects = true;
                    if (horizontal) {
                        intersection = {other.getValue(), v};
                    } else {
                        intersection = {v, other.getValue()};
                    }
                }
            }
        }
        return intersects;
    }
};

class Wire {
private:
    vector<WireSegment> segments;
public:
    explicit Wire(const string& wire_str) {
        stringstream ss;
        ss.str(wire_str);
        string s;
        vec2i p({0,0});
        size_t arc_length = 0;
        while(getline(ss, s, ',')) {
            segments.emplace_back(WireSegment(p, s, arc_length));
            p = segments.back().getEnd();
            arc_length = segments.back().getArcLength(p);
        }
    }
    const vector<WireSegment>& getSegments() const {
        return segments;
    }
};

int intersect(const Wire& w1, const Wire& w2, bool part_two = false) {
    int min_dist = 1000000;
    vec2i loc;
    int d;
    for (const WireSegment& ws1 : w1.getSegments()) {
        for (const WireSegment& ws2 : w2.getSegments()) {
            if (ws1.intersectsWith(ws2, loc)) {
                if (!part_two) {
                    // Part one: manhattan-distance of intersection from origin
                    d = abs(loc.first) + abs(loc.second);
                } else {
                    // Part two: total length of both wires between origin and intersection
                    d = ws1.getArcLength(loc) + ws2.getArcLength(loc);
                }
                if (d < min_dist) {
                    min_dist = d;
                }
            }
        }
    }
    return min_dist;
}

void day03(bool part_two) {
    cout << "AoC D2: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input03.txt");
    if (in.is_open()) {
        string ws1, ws2;
        getline(in, ws1);
        getline(in, ws2);
        Wire w1(ws1);
        Wire w2(ws2);
        cout << intersect(w1, w2, part_two) << endl;
    }
}

