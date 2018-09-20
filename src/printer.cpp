#include "printer.h"
#include <iostream>
#include <fstream>

void print_str(std::ostream& os, const std::string& s) {
	os << "	" << s << ";\n";
}

void print_gv(const Automata& aut, const std::string& filename) {
	std::ofstream os(filename, std::ofstream::out);

	os << "digraph minimized {\n";

	for (size_t term : aut.terminals) {
		print_str(os, std::to_string(term + 1) + " [peripheries=2]");
	}
	print_str(os, std::to_string(aut.start_vertex + 1) + " [label=\"" + std::to_string(aut.start_vertex + 1) + " (START)\"]");

	for (size_t v = 0; v < aut.V; ++v) {
		for (Edge u : aut.gr[v]) {
			print_str(os, std::to_string(v + 1) + " -> " + std::to_string(u.to + 1) + " [label=\"" + u.symb + "\"]");
		}
	}

	os << "}\n";

	os.close();
}