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

using namespace std;

int main() {
	// Allergen -> [Ingredient -> Count]
	unordered_map<string, unordered_map<string, int>> m;
	unordered_map<string, int> total_ingrs;

	for (string line; getline(cin, line);) {
		auto ppos = line.find("(contains");
		if (ppos == string::npos) continue;
		istringstream istream (line.substr(0, ppos));
		istringstream astream (line.substr(ppos+10));

		vector<string> ingrs;
		for (string ingr; istream >> ingr;) {
			ingrs.push_back(ingr);
			total_ingrs[ingr]++;
		}

		for (string all; astream >> all;) {
			auto lc = all[all.size() - 1];
			if (lc == ',' || lc == ')') all = all.substr(0, all.size() - 1);

			for (auto ingr : ingrs) m[all][ingr]++;
		}
	}

	while (accumulate(m.begin(), m.end(), 0, [](auto acc, auto x) { return acc + (x.second.size() > 1); })) {
		for (auto [all, ingrs] : m) {
			if (ingrs.size() < 2) continue;
			auto biggest = 0, second_biggest = 0;
			string biggest_ingr, second_biggest_ingr;
			for (auto [ingr, count] : ingrs) {
				if (count > biggest) {
					second_biggest = biggest; second_biggest_ingr = biggest_ingr;
					biggest = count; biggest_ingr = ingr;
				} else if (count > second_biggest) {
					second_biggest = count; second_biggest_ingr = ingr;
				}
			}

			if (biggest > second_biggest) {
				for (auto [all, ingrs] : m) {
					if (m[all].find(biggest_ingr) != m[all].end()) m[all].erase(m[all].find(biggest_ingr));
				}

				m[all] = {};
				m[all][biggest_ingr] = 1;
			}
		}

		cout << "-------------" << endl;
		for (auto [all, ingrs] : m) {
			cout << all << ":" << endl;
			for (auto [ingr, count] : ingrs) cout << "\t" << ingr << ": " << count << endl;
			cout << endl;
		}
	}

	set<string> allergens;
	for (auto [all, ingrs] : m) {
		for (auto [ingr, count] : ingrs) allergens.insert(ingr);
	}

	auto p1 = 0;
	for (auto [ingr, count] : total_ingrs) {
		if (find(allergens.begin(), allergens.end(), ingr) == allergens.end()) p1 += count;
	}
	cout << p1 << endl;

	for (auto ingr : allergens) cout << ingr << ',';
	cout << endl;
}
