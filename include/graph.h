#ifndef INCLUDE_GRAPH_H
#define INCLUDE_GRAPH_H

#include <string>
#include <vector>

struct Edge {
	size_t to;
	char symb;

	bool operator<(const Edge& other) const {
		if (to != other.to) return to < other.to;
		return symb < other.symb;
	}

	bool operator==(const Edge& other) const {
		return to == other.to && symb == other.symb;
	}
};

struct Automata {
	std::vector<char> alphabet;
	size_t V;
	size_t start_vertex;
	std::vector<size_t> terminals;
	std::vector<std::vector<Edge>> gr;

	Automata() : V(0), start_vertex(0) {}
	Automata(size_t V) : V(V), start_vertex(0) {
		gr.resize(V);
	}
	Automata(const Automata& other) : alphabet(other.alphabet), V(other.V), start_vertex(other.start_vertex),
													terminals(other.terminals), gr(other.gr) {}
};

Automata read_automata(const std::string& filename);
void write_automata(Automata& aut, const std::string& filename);
void print_automata(const Automata& aut, const std::string& gvname, const std::string& pngname);

#endif