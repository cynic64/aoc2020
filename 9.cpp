#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <cstdint>
#include <numeric>

using namespace std;

int main() {
	vector<unsigned long> lines;
	for (string line; getline(cin, line);) {
		lines.push_back(stoul(line));
	}

	auto n = 0;
	for (auto i = 25; i < lines.size(); ++i) {
		auto ok = false;
		for (auto j = i-25; j < i; ++j) {
			for (auto k = j; k < i; ++k) {
				if (lines[j] + lines[k] == lines[i])
					ok = true;
			}
		}
		if (!ok) {
			cout << lines[i] << endl;
			n = lines[i];
		}
	}

	auto q = 0, best = 0;
	for (auto i = 0; i < lines.size(); ++i) {
		for (auto j = i+1; j < lines.size(); ++j) {
			auto sum = accumulate(lines.begin()+i, lines.begin()+j, 0);
			unsigned long ma = *max_element(lines.begin()+i, lines.begin()+j);
			unsigned long mi = *min_element(lines.begin()+i, lines.begin()+j);
			/*
			for (auto k = i; k <= j; k++) {
				x = min(x, lines[k]);
				y = max(y, lines[k]);
				sum += lines[k];
			}
			*/
			if (sum == n && ((j-i+1) > best)) {
				best = ((j-i+1));
				q = ma + mi;
			}
		}
	}
	cout << q << endl;
}
