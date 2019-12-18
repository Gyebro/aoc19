//
// Created by Gyebro on 2019-12-16.
//
#include "day14.h"
#include <math.h>
#include <map>

struct chemical {
    string name;
    int64_t quantity;
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
            return {p[1],stoi(p[0])};
        } else {
            cout << "Error: '" << s << "' is not a valid chemical (quantity + name)\n";
            return {"NULL",0};
        }
    }
    pair<string, pair<int, vector<chemical>>> get_pair() {
        return {out.name, {out.quantity, in}};
    }
};

int64_t generate(map<string, pair<int, vector<chemical>>> reactions, chemical target, bool verbose = false) {
    map<string, int64_t> pool; // Current catalog of chemicals. Positive quantity is needed, negative quantity is unused available
    pool[target.name] = target.quantity;
    bool generating = true;
    pair<int, vector<chemical>> recipe;
    size_t i=0;
    bool settled = true;
    bool allow_unused_extra = false;
    while (generating) {
        if (verbose) cout << "==== STEP: " << i << "====\n";
        settled = true; // Assume i-th iteration is settled
        generating = false; // Assume nothing left to generate
        for (const auto& e : pool) {
            if (e.first == "ORE") {
                if (verbose) cout << "Pool requires: " << e.second << " " << e.first << "s\n";
            } else if (e.second > 0) { // Pool has required elements (other than ORE)
                generating = true; // We still have something to generate
                if (verbose) cout << "Need to generate " << e.second << " " << e.first << endl;
                recipe = reactions[e.first];
                // Find out how many times we need to apply recipe
                if (verbose) cout << " Recipe: " << recipe.first << " " << e.first << " needs " << recipe.second.size() << " ingredients\n";
                if (e.second >= recipe.first) {
                    size_t mul = e.second/recipe.first; // Rounds down
                    if (verbose) cout << " Applying recipe " << mul << " times\n";
                    settled = false; // If any recipe is used, current iteration is not settled
                    for (const chemical& c : recipe.second) {
                        pool[c.name] += mul*c.quantity; // Positive: we need the input ingredients
                    }
                    pool[e.first] -= mul*recipe.first; // Negative: decrease number of necessary outputs
                } else if (allow_unused_extra) {
                    // Apply recipe and introduced extra materials
                    if (verbose) cout << " Applying recipe once, introducing unused " << e.first << "\n";
                    settled = false; // If any recipe is used, current iteration is not settled
                    for (const chemical& c : recipe.second) {
                        pool[c.name] += c.quantity; // Positive: we need the input ingredients
                    }
                    pool[e.first] -= recipe.first; // Negative: decrease number of necessary outputs
                    // Clear flag
                    allow_unused_extra = false;
                } else {
                    if (verbose) cout << " Can't apply recipe\n";
                }
            } else if (e.second < 0) {
                if (verbose) cout << "Pool has " << -e.second << " extra " << e.first << endl;
            } else {
                // Quantity is zero now
            }
        }
        if (settled) {
            allow_unused_extra = true;
        }
        i++;
        cout << flush;
    }
    return pool["ORE"];
}


void day14(bool part_two) {
    cout << "AoC D14: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input14.txt");
    string line;
    vector<string> parts;
    map<string, pair<int, vector<chemical>>> reactions;
    if (in.is_open()) {
        while (getline(in, line)) {
            parts = split(line, '=');
            if (parts.size() == 2) {
                reaction r(reaction(parts[0],parts[1].substr(1)));
                reactions.insert(r.get_pair());
            }
        }
        int required = generate(reactions,{"FUEL", 1}, false); // 3rd arg: verbose
        if (!part_two) {
            cout << "1 FUEL requires: " << required << " ORE(s)\n";
        } else {
            // Try to search for the number of FUEL(s) which require 1000000000000 OREs (or slightly less)
            size_t ores = 1000000000000;
            int64_t min_fuels = ores/required; // Assume unit cost from creating one FUEL
            int64_t min_ores = generate(reactions,{"FUEL", min_fuels});
            int64_t fuels = ores/(min_ores/min_fuels);
            int64_t needed_ores = generate(reactions,{"FUEL", fuels});
            //cout << needed_ores << " OREs generate " << fuels << " FUELs\n";
            while (needed_ores < ores) {
                fuels++;
                needed_ores = generate(reactions,{"FUEL", fuels});
            }
            cout << ores << " can yield a maximum FUEL amount of: " << --fuels << endl;

        }
    }
}