#include "minimizer.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

void dfs(size_t v, std::vector<bool>& used, const Automata& aut) {
	if (used[v]) return;
	used[v] = true;
	for (const Edge& e : aut.gr[v])
		dfs(e.to, used, aut);
}

void rename_dfs(size_t v, std::vector<bool>& used, const Automata& aut, Automata& connected, const std::vector<size_t>& new_val) {
	if (used[v]) return;
	used[v] = true;
	for (const Edge& e : aut.gr[v]) {
		rename_dfs(e.to, used, aut, connected, new_val);
		connected.gr[new_val[v]].push_back({new_val[e.to], e.symb});
	}
}

Automata delete_unconnected(const Automata& aut) {
	std::vector<bool> used(aut.V, false);

	dfs(aut.start_vertex, used, aut);

	std::vector<size_t> new_val(aut.V);
	size_t ctr = 0;
	for (size_t i = 0; i < aut.V; ++i) {
		new_val[i] = ctr;
		ctr += used[i];
	}

	Automata connected(ctr);
	connected.alphabet = aut.alphabet;
	for (size_t v : aut.terminals)
		if (used[v])
			connected.terminals.push_back(new_val[v]);
	connected.start_vertex = new_val[aut.start_vertex];

	std::fill(used.begin(), used.end(), false);
	rename_dfs(aut.start_vertex, used, aut, connected, new_val);

	return connected;
}

Automata reverse(const Automata& aut) {
	Automata reversed(aut.V);
	reversed.alphabet = aut.alphabet;
	reversed.terminals = aut.terminals;
	reversed.start_vertex = aut.start_vertex;
	for (size_t v = 0; v < aut.V; ++v)
		for (Edge e : aut.gr[v])
			reversed.gr[e.to].push_back({v, e.symb});
	return reversed;
}

void color_dfs(size_t v, int32_t col, std::vector<int32_t>& color, const std::vector<std::vector<bool>>& equal) {
	if (color[v] != -1) return;
	color[v] = col;
	for (size_t u = 0; u < equal[v].size(); ++u)
		if (equal[v][u])
			color_dfs(u, col, color, equal);
}

void build_condensed(size_t v, std::vector<bool>& used, std::vector<int32_t>& color, const Automata& aut, Automata& result) {
	if (used[v]) return;
	used[v] = true;
	for (Edge e : aut.gr[v]) {
		build_condensed(e.to, used, color, aut, result);
		 result.gr[color[v]].push_back({(size_t) color[e.to], e.symb});
	}
}

void delete_same_sort(Automata& aut) {
	for (std::vector<Edge>& gr : aut.gr) {
		std::sort(gr.begin(), gr.end(), [](const Edge& a, const Edge& b) {
			return a.symb < b.symb;
		});
		gr.erase(std::unique(gr.begin(), gr.end()), gr.end());
	}
}

Automata minimization(const Automata& aut) {
	Automata reversed = reverse(aut);

	std::vector<std::vector<bool>> equal(aut.V, std::vector<bool>(aut.V, true));

	std::vector<size_t> not_terminal;
	for (size_t v = 0; v < aut.V; ++v) {
		if (std::find(aut.terminals.begin(), aut.terminals.end(), v) == aut.terminals.end()) {
			not_terminal.push_back(v);
		}
	}

	std::queue<std::pair<size_t, size_t>> Q;

	for (size_t v : aut.terminals) {
		for (size_t u : not_terminal) {
			equal[v][u] = equal[u][v] = false;
			Q.push({v, u});
		}
	}

	while (!Q.empty()) {
		auto [l, r] = Q.front();
		Q.pop();
		for (Edge e1 : reversed.gr[l]) {
			for (Edge e2 : reversed.gr[r]) {
				if (e1.symb == e2.symb && equal[e1.to][e2.to]) {
					equal[e1.to][e2.to] = equal[e2.to][e1.to] = false;
					Q.push({e1.to, e2.to});
				}
			}
		}
	}

	std::vector<int32_t> color(aut.V, -1);
	int32_t col = 0;
	for (size_t v = 0; v < aut.V; ++v) {
		if (color[v] == -1)
			color_dfs(v, col++, color, equal);
	}

	Automata result(col);
	result.alphabet = aut.alphabet;
	result.start_vertex = color[aut.start_vertex];

	std::vector<bool> used(aut.V, false);
	build_condensed(aut.start_vertex, used, color, aut, result);
	for (size_t u : aut.terminals)
		if (std::find(result.terminals.begin(), result.terminals.end(), color[u]) == result.terminals.end())
			result.terminals.push_back(color[u]);

	delete_same_sort(result);

	return result;
}

Automata minimize(const Automata& aut) {
	std::cerr << "Minimizing automata...\n";

	Automata connected = delete_unconnected(aut);

	Automata ret = minimization(connected);

	std::cerr << "Done!\n";

	return ret;
}