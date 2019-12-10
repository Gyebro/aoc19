//
// Created by Gyebro on 2019-12-10.
//
#include <cmath>
#include <algorithm>
#include "day10.h"

typedef pair<int, int> vec2i;

int sgn(int a) {
    return a>=0;
}

const double eps = 0.000001;

size_t scan(const vector<vec2i>& m, const vec2i& a) {
    int x=a.first; int y=a.second;
    size_t v = 0;
    int xi, yi, xb, yb;
    bool vis;
    double ai, ab;
    for (const vec2i& o : m) {
        xi=o.first; yi=o.second;
        if(!(x==xi && y==yi)) { // If o != a
            // check if o is visible from a
            // loop on other asteroids find blockers
            vis = true;
            for (const vec2i& b : m) {
                xb=b.first; yb=b.second;
                if (!vis) break;
                if(!(xb==xi && yb==yi) && !(xb==x &&  yb==y)) {
                    if (y==yi && y==yb) { // Check for horizontal block
                        if (abs(xb-x)<abs(xi-x) && sgn(xb-x)==sgn(xi-x)) {
                            vis = false;
                        }
                    } else if (y==yi || y==yb) {
                        // doesnt block, avoid 0 division
                    } else {
                        ai = static_cast<double>(xi-x)/static_cast<double>(yi-y);
                        ab = static_cast<double>(xb-x)/static_cast<double>(yb-y);
                        if (fabs(ai-ab)<eps) { // same angle with respect to a
                            if (fabs(ai)<eps) { // Check for vertical block
                                if(sgn(yb-y)==sgn(yi-y) && abs(yb-y)<abs(yi-y)) {
                                    vis=false;
                                }
                            } else if (sgn(xb-x)==sgn(xi-x) && abs(xb-x)<abs(xi-x)) {
                                vis=false;
                            }
                        }
                    }
                }
            } // for possible blockers
            if (vis) v++;
        } // if
    } // for
    return v;
}

struct asteroid_line {
    double angle;
    deque<vec2i> asteroids;
};

vec2i scan_and_destroy(vector<vec2i>& map, vec2i& base, const size_t to_destroy) {
    vector<asteroid_line> radial_map;
    size_t idx;
    for (const vec2i& a : map) {
        if (a != base) {
            int x = a.first - base.first; // a.x = X + base.x
            int y = -a.second + base.second; // a.y = -Y + base.y
            // Coord system is flipped now
            // ^ y
            // |
            // +--> x
            double ang = atan2(y, x);  // counterclockwise from {1,0} direction
            ang = -(ang - PI/2.0);     // clockwise from {0,1} direction
            if (ang < 0) ang += 2.0*PI;
            // Check if an asteroid line exists for this angle
            idx = find_idx_if(radial_map, [ang](const asteroid_line &al) { return fabs(al.angle - ang) < eps; });
            if (idx < radial_map.size()) {
                radial_map[idx].asteroids.push_back({x,y});
            } else {
                radial_map.push_back({ang, {{x,y}}});
            }
        } else {
            //cout << "Skipping base" << endl;
        }
    }
    // Sort the map
    sort(radial_map.begin(), radial_map.end(), [] (asteroid_line const& al1, asteroid_line const& al2) -> bool {
        return al1.angle < al2.angle;
    });
    // Sort lines within the map
    for (asteroid_line& al : radial_map) {
        sort(al.asteroids.begin(), al.asteroids.end(), [] (const vec2i& a1, const vec2i& a2) -> bool {
            return a1.first*a1.first+a1.second*a1.second < a2.first*a2.first+a2.second*a2.second;
        });
    }
    size_t destroyed = 1;
    for (asteroid_line& al : radial_map) {
        if (!al.asteroids.empty()) {
            vec2i dest = al.asteroids.front();
            //cout << destroyed << " Laser destroys : (" << dest.first+base.first << "," << -dest.second+base.second << ")\n";
            al.asteroids.pop_front();
            if (destroyed == to_destroy) {
                return {(dest.first+base.first),(-dest.second+base.second)};
            }
            destroyed++;
        }
    }
    return base;
}

void day10(bool part_two) {
    cout << "AoC D10: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input10.txt");
    string line;
    int x=0, y=0;
    vector<vec2i> map;
    if (in.is_open()) {
        while (getline(in, line)) {
            x=0;
            for (char a : line) {
                if (a == '#') map.push_back({x,y});
                x++;
            }
            y++;
        }
        //cout << "Asteroids on map: " << map.size() << endl;
        size_t max_visible=0;
        size_t visible;
        vec2i base;
        for (const vec2i& asteroid : map) {
            visible = scan(map,asteroid);
            if (visible > max_visible) {
                max_visible = visible;
                base = asteroid;
            }
        }
        if (!part_two) {
            cout << "Max visible: " << max_visible << " from (" << base.first << "," << base.second << ")" << endl;
        } else {
            vec2i ast = scan_and_destroy(map, base, 200);
            cout << "200th destroyed asteroid value is: " << 100*ast.first+ast.second << endl;
        }
    }
}
