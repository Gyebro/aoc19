//
// Created by Gyebro on 2019-12-05.
//

#include "days.h"
#include <iomanip>

using namespace std;

int main() {
    vector<string> tasks, tasks_terminal;
    vector<double> time_ms;

    vector<string> day_titles = {
            "The Tyranny of the Rocket Equation",
            "1202 Program Alarm",
            "Crossed Wires",
            "Secure Container",
            "Sunny with a Chance of Asteroids",
            "Universal Orbit Map",
            "Amplification Circuit",
            "Space Image Format",
            "Sensor Boost",
            "Monitoring Station",
            "Space Police",
            "The N-Body Problem",
            "Care Package",
            "Space Stoichiometry",
            "Oxygen System",
            "Flawed Frequency Transmission",
            "Set and Forget",
            "Many-Worlds Interpretation",
            "Tractor Beam",
            "Donut Maze",
            "Springdroid Adventure",
            "Slam Shuffle",
            "Category Six",
            "Planet of Discord",
            "Cryostasis"
    };

    void (*day_functions[])(bool) = {
            &day01, &day02, &day03, &day04, &day05,
            &day06, &day07, &day08, &day09, &day10,
            &day11, &day12, &day13, &day14, &day15,
            &day16, &day17, &day18, &day19, &day20,
            &day21, &day22, &day23, &day24, &day25
    };

    Clock c;
    string day, day_w_title, inputfile, num;

    for (size_t d = 1; d <= day_titles.size(); d++) {
        if (d < 10) { num = "0"+to_string(d); }
        else {        num = to_string(d); }
        cout << endl << "Day "+num << endl;
        tasks_terminal.emplace_back("Day "+num);
        tasks.emplace_back("Day "+num+": "+day_titles[d-1]);
        // Run part one
        c.start();
        day_functions[d-1](false);
        c.stop();
        time_ms.push_back(c.read_msec());
        c.start();
        day_functions[d-1](true);
        c.stop();
        time_ms.push_back(c.read_msec());
    }

    // Print result to cout
    cout << endl << endl;
    cout << "Task  \t\tP1 [ms]\t\tP2 [ms]\n";
    cout << "------\t\t-------\t\t-------\n";
    cout << setprecision(3) << fixed;
    for (size_t i=0; i<tasks.size(); i++) {
        cout << tasks_terminal[i] << "\t\t" << time_ms[2*i] << "\t\t" << time_ms[2*i+1] << '\n';
    }
    flush(cout);

    // Also print to readme.md
    ofstream out("../README.md");
    out << "# aoc19\n";
    out << "Advent of Code 2019 in C++\n";
    out << "## Computation times (no optimizations, both parts run separately and parse the input file).\n";
    out << "Processor: Intel Core i7-7700HQ, single thread unless indicated\n";
    out << endl;
    out << "Day | Part One [ms] | Part Two [ms]\n";
    out << "--- | ---: | ---:\n";
    out << setprecision(3) << fixed;
    double time_sum = 0;
    for (size_t i=0; i<tasks.size(); i++) {
        if (i != 25-1) {
            out << tasks[i] << " | " << time_ms[2*i] << " | " << time_ms[2*i+1] << '\n';
            time_sum += time_ms[2*i]+time_ms[2*i+1];
        } else {
            out << tasks[i] << " | " << time_ms[2*i] << " | - \n";
            time_sum += time_ms[2*i];
        }

    }
    out << endl;
    out << "Total time: " << time_sum/1000.0 << " seconds\n";
    out << endl;
    out << "## And so the warp drive has been aligned!\n";
    out << "![AoC19 calendar](calendar.gif)";
    out.close();
    return 0;
}