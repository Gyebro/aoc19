//
// Created by Gyebro on 2019-12-16.
//
#include "day16.h"

const vector<int> base_pattern = {0,1,0,-1};

// Returns the p-th element of gen_pattern(n) in place
int get_pattern(const size_t& n, const size_t& p) {
    return base_pattern[p/(n+1)];
}

// Used in the initial version (fft)
vector<int> gen_pattern(const size_t n) {
    // Repeat 0,1,0,-1 N times
    vector<int> p; p.reserve(4*(n+1));
    for (const int& b : base_pattern) {
        for (size_t i=0; i<n+1; i++) {
            p.push_back(b);
        }
    }
    return p;
}

// Slightly ptimized version
void fft2(const vector<int>& s, vector<int>& out) {
    if (out.size() != s.size()) out.resize(s.size());
    for (size_t i=0; i<s.size(); i++) {
        size_t psize = 4*(i+1);
        size_t p = 1; // Skip first element once
        int64_t val = 0;
        for (const int& elem : s) {
            val += elem*get_pattern(i,p);
            p++; if (p == psize) { p=0; }
        }
        out[i] = abs(val%10);
    }
}

// Initial version without optimizations
vector<int> fft(const vector<int>& s) {
    vector<int> pattern;
    vector<int> out(s.size());
    for (size_t i=0; i<s.size(); i++) {
        // Generate pattern:
        pattern = gen_pattern(i);
        // Apply the pattern in this pass
        size_t p = 1; // Skip first element once
        int64_t val = 0;
        for (const int& elem : s) {
            val += elem*pattern[p];
            p++; if (p == pattern.size()) { p=0; }
        }
        out[i] = abs(val%10);
    }
    return out;
}

// Only updates the last N elements
void fft_back(const vector<int>& s, std::vector<int>& out, size_t N) {
    const size_t len = s.size();
    // Accumulate the sum of digits from len-N to len
    int sum = 0;
    for (size_t i=len-N; i<len; i++) { sum += s[i]; }
    // Calculate output at 'i' if pattern is constant 1
    for (size_t i=len-N; i<len; i++) {
        out[i] = sum%10;
        sum -= s[i]; // Remove currently used digit from the sum
    }
}

void day16(bool part_two) {
    cout << "AoC D16: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input16.txt");
    string line;
    vector<int> sequence;
    string ch;
    if (in.is_open()) {
        getline(in,line);
        for (const char& c : line) {
            ch = c;
            sequence.push_back(stoi(ch));
        }
        if (!part_two) {
            size_t num_phases = 100;
            vector<int> output = sequence;
            for (size_t phases = 0; phases < num_phases; phases+=2) {
                fft2(sequence, output);
                fft2(output, sequence);
            }
            cout << "First 8 digits after " << num_phases << " phases: ";
            for (size_t i = 0; i < 8; i++) cout << sequence[i];
            cout << endl;
        } else {
            // Get offset
            string offset_str;
            for (size_t i = 0; i < 7; i++) offset_str += to_string(sequence[i]);
            size_t offset = stoi(offset_str);
            //cout << "Message offset: " << offset << endl;
            size_t repetitions=10000;
            vector<int> ext;
            ext.reserve(repetitions*sequence.size());
            for (size_t rep=0; rep<repetitions; rep++) {
                for (const int& i : sequence) {
                    ext.push_back(i);
                }
            }
            size_t back = ext.size()-offset;
            vector<int> out = ext;
            for (size_t phases = 0; phases < 100; phases+=2) {
                fft_back(ext, out, back);
                fft_back(out, ext, back);
            }
            cout << "Encoded message: ";
            for (size_t k=offset; k<offset+8; k++) {
                cout << ext[k];
            }
            cout << endl;
        }
    }
}

