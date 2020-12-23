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
	vector<int> mask (36, -1);
	unordered_map<uint64_t, uint64_t> mem;
	
	for (string line; getline(cin, line);) {
		smatch match;
		if (regex_match(line, match, regex {"mem\\[(\\d+)\\] = (\\d+)"})) {
			uint64_t idx = stoul(match[1]);
			uint64_t val = stoi(match[2]);
			printf("set %lu to %lu\n", idx, val);

			// Modify idx
			for (auto i = 0; i < 36; ++i) {
				if (mask[35-i] == 1) idx |= 1UL << i;
				else if (mask[35-i] == 0) ;
			}
			printf("Modified idx: %lu\n", idx);

			auto float_count = count(mask.begin(), mask.end(), -1);
			if (float_count == 0) mem[idx] = val;
			else {
				uint64_t versions = 1 << float_count;
				printf("Versions: %lu\n", versions);
				for (uint64_t i = 0; i < versions; ++i) {
					for (uint64_t j = 0, k = 0; k < float_count; ++j) {
						if (mask[35-j] == -1) {
							if ((i >> k) & 1) {
								idx |= 1UL << j;
							} else {
								idx &= ~(1UL << j);
							}
							++k;
						}
					};
					printf("set idx %lu to %lu\n", idx, val);
					mem[idx] = val;
				}
			}
		} else {
			auto pos = line.find(" = ");
			auto m = line.substr(pos + 3);
			for (auto i = 0; i < 36; ++i) {
				if (m[i] == '0') mask[i] = 0;
				else if (m[i] == '1') mask[i] = 1;
				else mask[i] = -1;
			}
			for (auto c : mask) cout << c << " ";
			cout << endl;
		}
	}

	uint64_t s = 0;
	for (auto [i, x] : mem) {
		if (x > 0) cout << x << endl;
		s += x;
		if (s > ((uint64_t)1 << 60)) {
			printf("big!\n");
		}
	}
	cout << "P1: " << s << endl;
}
