//
// Created by Gyebro on 2019-12-09.
//
#include "day08.h"

void day08(bool part_two) {
    cout << "AoC D8: part " << (part_two ? "two" : "one") << endl;
    const size_t w=25;
    const size_t h=6;
    vector<char> image(w*h,'2'); // Start with transparent image
    vector<char> layer(w*h);
    ifstream in("input08.txt");
    char c;
    size_t zeros, ones, twos;
    size_t min_zeros = 2 * w * h;
    size_t chksum;
    bool eof = false;
    if (in.is_open()) {
        while (in.good() && !eof){
            zeros=0; ones=0; twos=0;
            for (size_t i=0; i<w*h; i++) {
                if (in.peek() == '\n') {
                    eof = true; break;
                }
                in >> c;
                switch (c) {
                    case '0':
                        zeros++; layer[i]=c; break;
                    case '1':
                        ones++; layer[i]=c; break;
                    case '2':
                        twos++; layer[i]=c; break;
                    default:
                        break;
                }
                if (part_two) {
                    if (image[i] == '2') image[i]=layer[i];
                }
            }
            if (zeros < min_zeros && !eof) {
                chksum = ones*twos;
                min_zeros = zeros;
            }
        }
        if (!part_two) {
            cout << "Image checksum: " << chksum << endl;
        } else {
            for (size_t i=0; i<w*h; i++) {
                cout << (image[i]=='1'?'#':' ');
                if (i%w==w-1) cout << endl;
            }
            cout << endl;
        }
    }
}

