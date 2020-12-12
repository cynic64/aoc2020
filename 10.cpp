#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>
#include <cassert>

using namespace std;

unordered_map<uint64_t, uint64_t> lookup;

uint64_t arrange(vector<int> v, int start, int j) {
	auto key = j*v.size() + start;
	if (lookup.find(key) != lookup.end()) return lookup[key];

	if (start == v.size()) {
		lookup[key] = 1;
		return 1;
	}
	
	auto i = start;
	uint64_t c = 0;
	while (v[i] - j <= 3 && i < v.size()) {
		c += arrange(v, i+1, v[i]);
		i++;
	}

	lookup[key] = c;
	return c;
}

int main() {
	vector<int> adap;
	for (string line; getline(cin, line);) {
		auto n = stoi(line);
		adap.push_back(n);
	}

	vector<int> used;
	auto j = 0;
	int diffs[4] {0, 0, 0, 0};
	while (used.size() < adap.size()) {
		auto min_dist = 999;
		auto ad = -1;
		for (auto a : adap) {
			auto d = a - j;
			if (d >= 1 && d <= 3 && d < min_dist && find(used.begin(), used.end(), ad) == used.end()) {
				min_dist = d;
				ad = a;
			}
		}
		
		used.push_back(ad);
		diffs[ad - j]++;
		j = ad;
	}
	cout << j+3 << endl;
	diffs[3]++;
	for (auto x : diffs) cout << x << " ";
	cout << endl;
	cout << diffs[1] * diffs[3] << endl;
	cout << endl << endl;

	sort(adap.begin(), adap.end());
	auto count = arrange(adap, 0, 0);
	cout << count << endl;
	cout << UINT64_MAX << endl;
}
