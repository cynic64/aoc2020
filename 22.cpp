#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>
#include <cassert>
#include <cmath>
#include <numeric>
#include <variant>
#include <string_view>
#include <set>
#include <list>

using namespace std;

bool p1_wins(list<int>& p1, list<int>& p2) {
	set<pair<list<int>, list<int>>> seen_before;

	while (!p1.empty() && !p2.empty()) {
		/*
		cout << "P1:" << endl;
		for (auto c : p1) cout << c << endl;
		cout << "P2:" << endl;
		for (auto c : p2) cout << c << endl;
		cout << endl;
		*/
		if (seen_before.find({p1, p2}) != seen_before.end()) {
			/*
			cout << "SEEN BEFORE!" << endl;
			auto pos = seen_before.find({p1, p2});
			auto [sp1, sp2] = *pos;
			cout << "\tP1:" << endl;
			for (auto c : sp1) cout << "\t\t" << c << endl;
			cout << "\tP2:" << endl;
			for (auto c : sp2) cout << "\t\t" << c << endl;
			*/
			return true;
		}
		seen_before.insert({p1, p2});

		auto c1 = p1.front(), c2 = p2.front();
		auto p1_w = false;
		if (p1.front() < p1.size() && p2.front() < p2.size()) {
			auto sub_p1(p1); sub_p1.pop_front();
			auto sub_p2(p2); sub_p2.pop_front();
			while(sub_p1.size() > c1) sub_p1.pop_back();
			while(sub_p2.size() > c2) sub_p2.pop_back();

			if (sub_p1.size() < c1 || sub_p2.size() < c2) {
				cout << "SHLOOP!" << endl;
				return c1 > c2;
			}
			//printf("Recurse, c1: %d, c2: %d, p1: %lu, p2: %lu\n", c1, c2, sub_p1.size(), sub_p2.size());

			p1_w = p1_wins(sub_p1, sub_p2);
			//cout << "P1 wins: " << p1_w << endl;
		} else p1_w = p1.front() > p2.front();
		p1.pop_front();
		p2.pop_front();

		if (p1_w) { p1.push_back(c1); p1.push_back(c2); }
		else { p2.push_back(c2); p2.push_back(c1); }
	}

	//printf("1: %lu, 2: %lu\n", p1.size(), p2.size());
	return p1.size() > p2.size();
}

int main() {
	auto pl2 = false;
	list<int> p1;
	list<int> p2;
	for (string line; getline(cin, line);) {
		if (line.size() == 0) {
			pl2 = true;
			continue;
		}
		if (line[0] == 'P') continue;

		auto c = stoi(line);
		if (pl2) p2.push_back(c);
		else p1.push_back(c);
	}

	p1_wins(p1, p2);

	if (p1.empty()) p1 = p2;

	uint64_t x = 1;
	uint64_t c = 0;
	for (auto i = p1.rbegin(); i != p1.rend(); ++i) {
		cout << *i << endl;
		c+= *i * (x++);
	}
	cout << c << endl;
}
