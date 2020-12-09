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
	for (auto i = lines.begin() + 25; i != lines.end(); ++i) {
		auto ok = false;
		for (auto j = i-25; j != i; ++j) {
			for (auto k = j; k != i; ++k) {
				if (*j + *k == *i)
					ok = true;
			}
		}
		if (!ok) {
			cout << *i << endl;
			n = *i;
		}
	}

	auto q = 0, best = 0;
	for (auto i = lines.begin(); i != lines.end(); ++i) {
		auto sum = 0;
		auto ma = *i;
		auto mi = *i;
		for (auto j = i+1; j != lines.end(); ++j) {
			sum += *j;
			ma = max(ma, *j);
			mi = min(mi, *j);

			if (sum == n && j-i > best) {
				best = j-i;
				q = ma + mi;
			}
		}
	}
	cout << q << endl;
}
