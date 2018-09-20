#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <ctime>
#include <string>

#include "graph.h"
#include "minimizer.h"
#include "printer.h"

struct Request {
    std::string input = "default/in.gr";
    std::string output = "default/out.gr";
    std::string save = "default/out.gv";
    std::string print = "default/outpr.png";
    bool print_enable = true;
};

void check(size_t argv, size_t pos) {
    if (pos + 1 >= argv) {
        std::cerr << "Bad input! Option was called without parameter!\n";
        std::abort();
    }
}

Request parse(size_t argv, char ** argc) {
    Request req;
    for (size_t pos = 1; pos < argv; ++pos) {
        std::string cur_option = std::string(argc[pos]);
        if (cur_option[0] == '-' && cur_option != "-n")
            check(argv, pos);
        if (cur_option == "-i")
            req.input = std::string(argc[++pos]);
        else if (cur_option == "-o")
            req.output = std::string(argc[++pos]);
        else if (cur_option == "-p")
            req.print = std::string(argc[++pos]);
        else if (cur_option == "-s")
            req.save = std::string(argc[++pos]);
        else if (cur_option == "-n")
            req.print_enable = false;
    }

    return req;
}

int main(int argv, char ** argc) {
    Request req = parse(argv, argc);
    Automata aut = read_automata(req.input);
    Automata minimized = minimize(aut);
    //Automata minimized = aut;
    write_automata(minimized, req.output);
    if (req.print_enable)
        print_automata(minimized, req.save, req.print);
    return 0;
}