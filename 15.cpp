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
	vector<int> lines;
	for (string line; getline(cin, line, ',');) {
		lines.push_back(stoi(line));
	}

	// number -> (before-last-said-index, last-said-index)
	unordered_map<int, array<int, 2>> spoken;

	auto idx = 0;
	for (auto line : lines) {
		spoken[line] = {-1, idx};
		++idx;
	}

	auto last = lines[idx-1];
	while (idx < 30000000) {
		if (idx % 1000000 == 0) cout << idx << endl;
		auto s = spoken.find(last);
		if (s != spoken.end()) { // Has been said before
			if (spoken[last][0] == -1) { // It was the first time
				//cout << 0 << endl;
				last = 0;
				spoken[0][0] = spoken[0][1];
				spoken[0][1] = idx;
			} else { // It had been said even before last time
				auto diff = spoken[last][1] - spoken[last][0];
				//cout << diff << endl;
				last = diff;

				auto s2 = spoken.find(diff);
				if (s2 != spoken.end()) {
					spoken[diff][0] = spoken[diff][1];
					spoken[diff][1] = idx;
				} else {
					spoken[diff] = {-1, idx};
				}
			}
		} else {
			cout << "what the fuck" << endl;
		}
		++idx;
	}
	cout << last << endl;
}
