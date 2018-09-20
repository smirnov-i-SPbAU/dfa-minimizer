#include "graph.h"
#include "printer.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <map>

bool good_symb(char c) {
	return ('a' <= c && c <= 'z') || ('0' <= c && c <= '9');
}

Automata read_automata(const std::string& filename) {
	std::cerr << "Reading automata...\n";

	std::ifstream is(filename, std::ifstream::in);

	Automata aut;

	std::string alpha;
	is >> alpha;
	std::map<char, bool> used;

	for (char c : alpha) {
		if (used[c] || !good_symb(c)) {
			std::cerr << "Bad alphabet!\n";
			abort();
		}
		used[c] = true;
		aut.alphabet.push_back(c);
	}

	is >> aut.V;
	++aut.V;

	is >> aut.start_vertex;
	--aut.start_vertex;

	size_t terminalsN;
	is >> terminalsN;
	aut.terminals.resize(terminalsN);
	for (size_t& v : aut.terminals) {
		is >> v;
		--v;
	}

	aut.gr.resize(aut.V);
	for (size_t v = 0; v < aut.V - 1; ++v) {
		for (char c : aut.alphabet) {
			size_t u;
			is >> u;
			--u;
			aut.gr[v].push_back({u, c});
		}
	}

	for (char c : aut.alphabet) {
		aut.gr[aut.V - 1].push_back({aut.V - 1, c});
	}
	is.close();

	std::cerr << "Done!\n";

	return aut;
}

void write_automata(Automata& aut, const std::string& filename) {
	std::cerr << "Writing automata...\n";

	std::ofstream os(filename, std::ofstream::out);

	std::sort(aut.alphabet.begin(), aut.alphabet.end());

	for (char c : aut.alphabet) {
		os << c;
	}
	os << '\n';

	os << aut.V << '\n';

	os << aut.start_vertex + 1 << '\n';

	os << aut.terminals.size();
	for (size_t v : aut.terminals)
		os << ' ' << v + 1;
	os << '\n';

	for (std::vector<Edge>& gr : aut.gr) {
		for (const Edge& e : gr) {
			os << e.to + 1 << ' ';
		}
		os << '\n';
	}

	os.close();

	std::cerr << "Done!\n";
}

void print_automata(const Automata& aut, const std::string& gvname, const std::string& pngname) {
	std::cerr << "Printing automata...\n";

	print_gv(aut, gvname);

	std::string req = "dot -Tpng " + gvname + " > " + pngname;

	if (std::system(req.c_str())) {
		std::cerr << "Something wrong with system call!\n";
		abort();
	}

	std::cerr << "Done!\n";
}