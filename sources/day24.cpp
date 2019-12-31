//
// Created by Gyebro on 2019-12-31.
//
#include "day24.h"
#include <bitset>

typedef bitset<25> eris_type;

eris_type evolve(eris_type e, eris_type prev, eris_type next) {
    size_t i = 0;
    size_t a = 0; // adjacent bugs
    eris_type r(0);
    for (size_t y=0; y<5; y++) {
        for (size_t x=0; x<5; x++) {
            a=0;
            i=y*5+x;
            // same level neighbours
            if (x>0) a+=e[i-1];
            if (x<4) a+=e[i+1];
            if (y>0) a+=e[i-5];
            if (y<4) a+=e[i+5];
            // prev / outer level
            if (x==0) a+=prev[11];
            if (x==4) a+=prev[13];
            if (y==0) a+=prev[7];
            if (y==4) a+=prev[17];
            // next / inner level
            if (i==7) {
                a+=next[0];
                a+=next[1];
                a+=next[2];
                a+=next[3];
                a+=next[4];
            } else if (i==17) {
                a+=next[20];
                a+=next[21];
                a+=next[22];
                a+=next[23];
                a+=next[24];
            } else if (i==11) {
                a+=next[0];
                a+=next[5];
                a+=next[10];
                a+=next[15];
                a+=next[20];
            } else if (i==13) {
                a+=next[4];
                a+=next[9];
                a+=next[14];
                a+=next[19];
                a+=next[24];
            }
            if (e[i]==1) { // bug
                r[i] = (a==1);
                // stays alive if a=1, dies otherwise
            } else { // empty
                r[i] = (a==1 || a==2);
                // becomes infested if a=1|2
            }
        } //for x
    } //for y
    r[12]=0; // clear middle
    return r;
}

vector<eris_type> evolve_rec(size_t t,vector<eris_type> in) {
    if(t>240) cout << "Error, maximum number of supported layers reached!";
    vector<eris_type> out(in.size(),eris_type(0));
    for(size_t i=250-t;i<=250+t;i++) {
        out[i]=evolve(in[i],in[i-1],in[i+1]);
    }
    return out;
}

eris_type evolve(eris_type e) {
    size_t i = 0;
    size_t a = 0; // adjacent bugs
    eris_type r(0);
    for (size_t y=0; y<5; y++) {
        for (size_t x=0; x<5; x++) {
            a=0;
            i=y*5+x;
            if (x>0) a+=e[i-1];
            if (x<4) a+=e[i+1];
            if (y>0) a+=e[i-5];
            if (y<4) a+=e[i+5];
            if (e[i]==1) { // bug
                r[i] = (a==1);
                // stays alive if a=1, dies otherwise
            } else { // empty
                r[i] = (a==1 || a==2);
                // becomes infested if a=1|2
            }
        } //for x
    } //for y
    return r;
}

void print_eris(eris_type e) {
    size_t i;
    for (size_t y=0; y<5; y++) {
        for (size_t x=0; x<5; x++) {
            i=y*5+x;
            cout<<(e[i] ? '#' : '.');
        } //for x
        cout<<endl;
    } //for y
}

void day24(bool part_two) {
    cout << "AoC D24: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input24.txt");
    size_t i=0;
    eris_type eris(0);
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            for (char c:line) {
                eris[i] = (c == '#');
                i++;
            }
        }
        //cout << "eris biodiversity score: ";
        //cout << eris.to_ulong() << endl;
        if (!part_two) {
            vector<eris_type> layouts(10000,eris_type(0));
            layouts.push_back(eris);
            bool scan = true;
            while (scan) {
                eris=evolve(eris);
                for (eris_type &e : layouts) {
                    if (e==eris) scan=false;
                }
                layouts.push_back(eris);
            }
            cout << "first repeating eris biodiversity score: ";
            cout << eris.to_ulong() << endl;
        } else {
            // part two
            // 200*2+1 layers should be enough for 200 time steps
            vector<eris_type> layers(501,eris_type(0));
            layers[250] = eris;
            for(size_t t=1; t<=200; t++) {
                layers=evolve_rec(t,layers);
            }
            size_t cnt = 0;
            for(eris_type &e : layers) {
                cnt += e.count();
            }
            cout << "total number of bugs: " << cnt << endl;
        }
    }
}


