//
// Created by Gyebro on 2019-12-05.
//
#include "day06.h"
#include <limits>

class Planet {
public:
    Planet(const string& ID) : id(ID) {
        master = numeric_limits<size_t>::max();
    }
    Planet(const string& ID, size_t master_idx) : id(ID), master(master_idx) { }
    string id;
    size_t master;
};

size_t get_orbit_number(const vector<Planet>& galaxy, const Planet& planet) {
    if (planet.master == numeric_limits<size_t>::max()) {
        return 0;
    } else {
        size_t orbits = 0;
        size_t master_idx = planet.master;
        while (master_idx != numeric_limits<size_t>::max()) {
            orbits++;
            master_idx = galaxy[master_idx].master;
        }
        return orbits;
    }
}

vector<size_t> get_master_sequence(const vector<Planet>& galaxy, const size_t& idx) {
    vector<size_t> path;
    size_t master_idx = galaxy[idx].master;
    while (master_idx != numeric_limits<size_t>::max()) {
        path.push_back(master_idx);
        master_idx = galaxy[master_idx].master;
    }
    return path;
}

void day06(bool part_two) {
    cout << "AoC D6: part " << (part_two ? "two" : "one") << endl;
    ifstream in("input06.txt");
    vector<Planet> galaxy;
    if (in.is_open()) {
        string line;
        size_t master_idx;
        size_t slave_idx;
        while (getline(in, line)) {
            string master_planet = line.substr(0, 3);
            string slave_planet  = line.substr(4);
            master_idx = find_idx_if(galaxy, [master_planet] (const Planet& p) { return p.id == master_planet; });
            if (master_idx != galaxy.size()) {
                // Master planet already exists
            } else {
                // Create master planet
                //cout << "Adding planet: " << master_planet << endl;
                galaxy.push_back(Planet(master_planet));
                master_idx = galaxy.size()-1;
            }
            slave_idx  = find_idx_if(galaxy,  [slave_planet] (const Planet& p) { return p.id == slave_planet; });
            if (slave_idx != galaxy.size()) {
                // Slave planet already exists, set its master
                if (galaxy[slave_idx].master == numeric_limits<size_t>::max()) {
                    galaxy[slave_idx].master = master_idx;
                } else {
                    cout << "Error: slave planet already has a master!\n";
                }
            } else {
                // Add slave planet
                //cout << "Adding planet: " << slave_planet << " which orbits " << master_planet << endl;
                galaxy.push_back(Planet(slave_planet, master_idx));
            }
        }
        cout << "Galaxy size: " << galaxy.size() << endl;
        cout << "Root planet(s): ";
        for (const Planet& p : galaxy) {
            if (p.master == numeric_limits<size_t>::max()) {
                cout << p.id << " ";
            }
        }
        cout << endl;
        if (!part_two) {
            size_t total_orbits = 0;
            for (const Planet& p : galaxy) {
                total_orbits += get_orbit_number(galaxy, p);
            }
            cout << "Total number of orbits: " << total_orbits << endl;
        } else {
            // Find SAN and YOU
            size_t you_idx = find_idx_if(galaxy, [] (const Planet& p) { return p.id == "YOU"; });
            size_t san_idx = find_idx_if(galaxy, [] (const Planet& p) { return p.id == "SAN"; });
            if (you_idx < galaxy.size() && san_idx < galaxy.size()) {
                cout << "Planets 'YOU' and 'SAN' found!\n";
                // Try to backtrack from both leaves until a common node is reached
                vector<size_t> you_path = get_master_sequence(galaxy, you_idx);
                vector<size_t> san_path = get_master_sequence(galaxy, san_idx);
                for (size_t you_step = 0; you_step < you_path.size(); you_step++) {
                    for (size_t san_step = 0; san_step < san_path.size(); san_step++) {
                        if (you_path[you_step] == san_path[san_step]) {
                            cout << "Number of orbital transfers needed: " << you_step + san_step << endl;
                            return;
                        }
                    }
                }
            }
        }
    }
}
