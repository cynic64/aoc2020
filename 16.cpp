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

using namespace std;

int main() {
	// name -> inclusive ranges
	unordered_map<string, vector<pair<int, int>>> rules;

	vector<int> your_ticket;
	vector<vector<int>> nearby_tickets;
	
	// 0 = rule scanning, 1 = ticket scanning
	auto mode = 0;
	for (string line; getline(cin, line); ) {
		if (line == "your ticket:") { mode = 1; continue; }
		if (line == "nearby tickets:") { continue; }
			 
		if (mode == 0) {
			smatch match;
			if (regex_match(line, match, regex {"(.+): (\\d+?)-(\\d+) or (\\d+?)-(\\d+?)"})) {
				auto name = match[1];
				auto r1 = stoi(match[2]);
				auto r2 = stoi(match[3]);
				auto r3 = stoi(match[4]);
				auto r4 = stoi(match[5]);
				rules[name] = {{r1, r2}, {r3, r4}};
			}
		} else if (mode == 1 && line.size() > 0) {
			smatch match;
			vector<int> ticket;
			while (regex_search(line, match, regex {"(\\d+)"})) {
				ticket.push_back(stoi(match[1]));
				line = match.suffix().str();
			}

			nearby_tickets.push_back(ticket);
		}

	}

	your_ticket = nearby_tickets[0];
	nearby_tickets.erase(nearby_tickets.begin());

	for (auto [name, ranges] : rules) {
		cout << "Name: " << name << endl;
		for (auto [min, max] : ranges) printf("\t%d-%d\n", min, max);
	}

	printf("Yours:\n");
	for (auto x : your_ticket) printf("%d ", x);
	printf("\n\n");

	printf("Nearby:\n");
	for (auto t : nearby_tickets) {
		for (auto x : t) printf("%d ", x);
		printf("\n");
	}

	auto fail = 0;
	vector<vector<int>> good_tickets;
	for (auto ticket : nearby_tickets) {
		auto all_ok = true;
		for (auto num : ticket) {
			auto works = false;
			for (auto [name, ranges] : rules) {
				for (auto [min, max] : ranges) {
					if (num >= min && num <= max) {
						works = true;
					}
				}
			}

			if (!works) { fail += num; all_ok = false; }
		}
		if (all_ok) good_tickets.push_back(ticket);
	}

	cout << "P1: " << fail << endl;

	// idx -> list of keys into rules that are possible for this idx
	unordered_map<int, vector<string>> possible_rules;

	auto max_idx = good_tickets[0].size();
	for (size_t i = 0; i < max_idx; ++i) {
		for (auto [name, ranges] : rules) {
			possible_rules[i].push_back(name);
		}
	}

	while (count_if(possible_rules.begin(), possible_rules.end(), [](auto p){ return p.second.size() == 1; }) != possible_rules.size()) {
		printf("Iter\n");
		for (size_t i = 0; i < max_idx; ++i) {
			printf("Possible rules for idx %lu: ", i);
			for (auto r : possible_rules[i]) printf("'%s' ", r.c_str());
			printf("\n");
		}

		for (size_t i = 0; i < max_idx; ++i) {
			vector<string> to_delete;
			for (auto name : possible_rules[i]) {
				// cout << "Rule name: " << name << endl;
				auto rule_works_for_all_tickets = true;
				for (auto ticket : good_tickets) {
					auto any_range_works_for_ticket = false;
					for (auto [min, max] : rules[name]) {
						if (ticket[i] >= min && ticket[i] <= max) {
							any_range_works_for_ticket = true;
						}
					}
					if (!any_range_works_for_ticket) rule_works_for_all_tickets = false;
				}

				if (!rule_works_for_all_tickets) {
					to_delete.push_back(name);
				}
			}

			for (auto name : to_delete) {
				// printf("Deleting '%s' for idx %lu\n", name.c_str(), i);
				auto pos = find(possible_rules[i].begin(), possible_rules[i].end(), name);
				possible_rules[i].erase(pos);
			}

			if (possible_rules[i].size() == 1) {
				auto name = possible_rules[i][0];
				for (size_t j = 0; j < max_idx; ++j) {
					if (i == j) continue;
					// printf("Deleting '%s' for idx %lu because someone else needs it\n", name.c_str(), j);
					auto pos = find(possible_rules[j].begin(), possible_rules[j].end(), name);
					if (pos == possible_rules[j].end()) continue;
					possible_rules[j].erase(pos);
				}
			}

			if (possible_rules[i].size() == 0) {
				printf("We fucked up, index %lu has no possible rules\n", i);
				return -1;
			}

			printf("\n");
		}
	}

	for (size_t i = 0; i < max_idx; ++i) {
		printf("Index %lu is: %s\n", i, possible_rules[i][0].c_str());
	}

	uint64_t res = 1;
	for (size_t i = 0; i < max_idx; ++i) {
		if (possible_rules[i][0].find("departure") != string::npos) {
			cout << possible_rules[i][0] << " " << your_ticket[i] << endl;
			res *= your_ticket[i];
		}
	}

	cout << res << endl;
}
