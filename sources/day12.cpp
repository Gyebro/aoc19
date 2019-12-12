//
// Created by Gyebro on 2019-12-12.
//
#include "day12.h"
#include <math.h>

int64_t sgn0(const int64_t i) {
    if (i == 0) {
        return 0;
    } else if (i>0) {
        return +1;
    } else {
        return -1;
    }
}

struct vec3i {
    int64_t x;
    int64_t y;
    int64_t z;
    const int64_t& operator[](size_t i) const {
        if (i==0) return x;
        else if (i==1) return y;
        else return z;
    }
};

vec3i operator+(const vec3i& a, const vec3i& b) {
    return {a.x+b.x, a.y+b.y, a.z+b.z};
}

vec3i operator-(const vec3i& a, const vec3i& b) {
    return {a.x-b.x, a.y-b.y, a.z-b.z};
}

struct moon {
    vec3i pos;
    vec3i vel;
};

void gravity(moon& m, moon& n) {
    int64_t x = sgn0(n.pos.x-m.pos.x);
    int64_t y = sgn0(n.pos.y-m.pos.y);
    int64_t z = sgn0(n.pos.z-m.pos.z);
    m.vel.x += x;
    m.vel.y += y;
    m.vel.z += z;
    n.vel.x -= x;
    n.vel.y -= y;
    n.vel.z -= z;
}

void move(moon& m) {
    m.pos.x += m.vel.x;
    m.pos.y += m.vel.y;
    m.pos.z += m.vel.z;
}

size_t total_energy(const moon& m) {
    size_t pot = abs(m.pos.x)+abs(m.pos.y)+abs(m.pos.z);
    size_t kin = abs(m.vel.x)+abs(m.vel.y)+abs(m.vel.z);
    return pot*kin;
}

void timestep(moon& m0, moon& m1, moon& m2, moon& m3) {
    gravity(m0, m1);
    gravity(m0, m2);
    gravity(m0, m3);
    gravity(m1, m2);
    gravity(m1, m3);
    gravity(m2, m3);
    move(m0);
    move(m1);
    move(m2);
    move(m3);
}

pair<size_t, size_t> operator+(const pair<size_t, size_t>& a, const pair<size_t, size_t>& b) {
    return {a.first+b.first, a.second+b.second};
}

check(const moon& m0, const moon& m1, const moon& m2, const moon& m3,
      const moon& m00, const moon& m01, const moon& m02, const moon& m03, size_t i) {
    return (m0.pos[i]==m00.pos[i]) && (m1.pos[i]==m01.pos[i]) && (m2.pos[i]==m02.pos[i]) && (m3.pos[i]==m03.pos[i]) &&
           (m0.vel[i]==m00.vel[i]) && (m1.vel[i]==m01.vel[i]) && (m2.vel[i]==m02.vel[i]) && (m3.vel[i]==m03.vel[i]);
}

void day12(bool part_two) {
    cout << "AoC D12: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input12.txt");
    // TODO: Parse
    moon m0({{-6,-5,-8},{0,0,0}});
    moon m1({{0, -3, -13},{0,0,0}});
    moon m2({{-15, 10, -11},{0,0,0}});
    moon m3({{-3, -8, 3},{0,0,0}});
    if (!part_two) {
        size_t time = 0;
        for (size_t time=0; time<1000; time++) {
            timestep(m0,m1,m2,m3);
        }
        cout << "Total energy of the system is: "
             << total_energy(m0)+total_energy(m1)+total_energy(m2)+total_energy(m3) << endl;
    } else {
        moon m00 = m0;
        moon m10 = m1;
        moon m20 = m2;
        moon m30 = m3;
        // Try to find a period in x, y and z separately
        bool searching = true;
        size_t time = 0;
        size_t px = 0, py = 0, pz = 0;
        size_t tx = 0, ty = 0, tz = 0;
        bool px_found = false;
        bool py_found = false;
        bool pz_found = false;
        while (searching) {
            timestep(m0,m1,m2,m3);
            time++;
            if (!px_found && check(m0,m1,m2,m3,m00,m10,m20,m30,0)) {
                px = (time-tx); px_found = true;
                //cout << "px = " << px << endl;
            }
            if (!py_found && check(m0,m1,m2,m3,m00,m10,m20,m30,1)) {
                py = (time-ty); py_found = true;
                //cout << "py = " << py << endl;
            }
            if (!pz_found && check(m0,m1,m2,m3,m00,m10,m20,m30,2)) {
                pz = (time-tz); pz_found = true;
                //cout << "pz = " << pz << endl;
            }
            if (px_found && py_found && pz_found) {
                searching = false;
            }
        }
        size_t period = lcm(px,py,pz);
        cout << "Period of 4-moon system: " << period << endl;
    }
}