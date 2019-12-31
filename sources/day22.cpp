//
// Created by Gyebro on 2019-12-23.
//
#include "day22.h"

typedef __int128 bigint;

// Part one, track card at index n if deck size is N

void dns(const size_t N, size_t& n) {
    n = (N-1)-n;
}

void cut(const size_t N, size_t& n, const int a) {
    if (a>0) { //cut positive
        size_t m=N-a;
        if (n<a) { // move back by m
            n+=m;
        } else { // move fwd by a
            n-=a;
        }
    } else { //cut neg
        size_t m=N+a; // m<N
        if (n<m) { // move back by a
            n-=a;
        } else { // move fwd by m
            n-=m;
        }
    }
}

void dwi(const size_t N, size_t& n, const size_t i) {
    n = (i*n)%N;
}

bigint mod(const bigint& a, const bigint& b) {
    return (a >= 0) ? (a % b) : (b + a % b);
}

bigint modpow(bigint base, bigint exponent, bigint n) {
    if (exponent == 0) {
        return (base == 0) ? 0 : 1;
    }
    bigint bit = 1;
    bigint power = mod(base, n);
    bigint out = 1;
    while (bit <= exponent) {
        if (exponent & bit) {
            out = mod(out * power, n);
        }
        power = mod(power * power, n);
        bit <<= 1;
    }
    return out;
}

bigint gcd_ext(bigint a, bigint b, bigint& x, bigint& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    bigint x1, y1;
    bigint gcd = gcd_ext(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

bigint modinv(const bigint &b, const bigint &n) {
    bigint x, y;
    bigint g = gcd_ext(b, n, x, y);
    return (g != 1) ? -1 : mod(x, n);
}

bigint moddiv(bigint a, bigint b, bigint n) {
    a = mod(a, n);
    bigint inv = modinv(b, n);
    return (inv == -1) ? -1 : (a * inv) % n;
}

string to_string(__int128 num) {
    string str, pre = "";
    if (num < 0) {
        num *= -1;
        pre = "-";
    }
    do {
        int64_t digit = num % 10;
        str = to_string(digit) + str;
        num = (num - digit) / 10;
    } while (num != 0);
    return pre + str;
}

void cut(const bigint M, bigint& offset, bigint& increment, bigint n) {
    if (n < 0) {n += M;}
    bigint a = 1; // increment stays 1
    bigint b = M - n; // offset shifted by n
    offset = mod(a * offset + b, M);
    increment = mod(a * increment, M);
}

void dwi(const bigint M, bigint& offset, bigint& increment, bigint n) {
    bigint a = n; // increment is n
    bigint b = 0; // offset stays 0
    offset = mod(a * offset + b, M);
    increment = mod(a * increment, M);
}

void dns(const bigint M, bigint& offset, bigint& increment) {
    bigint a = -1; // increment is reversed
    bigint b = M - 1; // offset is the end of deck
    offset = mod(a * offset + b, M);
    increment = mod(a * increment, M);
}

void day22(bool part_two) {
    cout << "AoC D22: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input22.txt");
    if (in.is_open()) {
        string line;
        int n;
        if (!part_two) {
            const size_t N = 10007;
            size_t idx = 2019;
            while(getline(in,line)) {
                if (line.substr(0,3)=="cut") {
                    n=stoi(line.substr(4));
                    cut(N,idx,n);
                } else if (line.substr(5,4)=="into") {
                    dns(N,idx);
                } else if (line.substr(5,4)=="with") {
                    n=stoi(line.substr(20));
                    dwi(N,idx,n);
                }
            }
            cout << "index of '2019' after one shuffle: " << idx << endl;
        } else {
            bigint M=119315717514047;
            bigint S=101741582076661;
            // Initial offset and increment
            bigint off=0;
            bigint inc=1;
            while(getline(in,line)) {
                if (line.substr(0,3)=="cut") {
                    n=stoi(line.substr(4));
                    cut(M,off,inc,n);
                } else if (line.substr(5,4)=="into") {
                    dns(M,off,inc);
                } else if (line.substr(5,4)=="with") {
                    n=stoi(line.substr(20));
                    dwi(M,off,inc,n);
                }
            }
            // Final increment is trivial
            bigint finalInc = modpow(inc, S, M);
            // Final offset is from geometric series expansion
            bigint finalOff = mod(off * moddiv(finalInc - 1, inc - 1, M), M);
            bigint idx = 2020;
            bigint cardValue = mod(moddiv(mod(idx - finalOff, M), finalInc, M), M);
            cout << "card at index '" << to_string(idx) << "' is: " << to_string(cardValue) << endl;
        }
    }
}

