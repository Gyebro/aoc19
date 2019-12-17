//
// Created by Gyebro on 2019-12-16.
//
#include "day14.h"

struct chemical {
    string name;
    size_t quantity;
};

class reaction {
public:
    vector<chemical> in;
    chemical out;
    reaction(const string& inputs, const string& output) {
        vector<string> chems = split(inputs,',');
        for (const string& i : chems) {
            in.push_back(parse_chemical(i));
        }
        out = parse_chemical(output);
    }
    chemical parse_chemical(const string& s) {
        vector<string> p = split(trim_spaces(s), ' ');
        if (p.size() == 2) {
            return {p[1],stoul(p[0])};
        } else {
            cout << "Error: '" << s << "' is not a valid chemical (quantity + name)\n";
            return {"NULL",0};
        }
    }
};

vector<chemical> generate_at_least(const chemical& c, const reaction& r) {
    vector<chemical> ret;
    if (c.name != r.out.name) {
        cout << "Error: invalid request\n";
    } else {
        // TODO: Decompose c using r, bool flag to allow introduction of extra c-s if needed
    }
    return ret;
}

void generate_chemical(const vector<reaction>& reactions, const chemical& target) {
    bool generating = true;
    vector<chemical> pool;
    pool.push_back(target);
    while (generating) {
        vector<chemical> new_pool;
        for (const chemical& c : pool) {
            cout << "Trying to decompose: " << c.quantity << " " << c.name << (c.quantity==1?"":"s") << endl;
            for (const reaction& r : reactions) {
                if (r.out.name == c.name) {
                    vector<chemical> res = generate_at_least(c, r);
                    for (const chemical& r : res) new_pool.push_back(r);
                }
            }
        }
        generating = false;
    }
}

void day14(bool part_two) {
    cout << "AoC D14: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input14.txt");
    string line;
    vector<string> parts;
    vector<reaction> reactions;
    if (in.is_open()) {
        while (getline(in, line)) {
            parts = split(line, '=');
            if (parts.size() == 2) {
                reactions.emplace_back(reaction(parts[0],parts[1].substr(1)));
            }
        }
        cout << "Found " << reactions.size() << " reactions\n";
        generate_chemical(reactions, {"FUEL",1});
    }
}