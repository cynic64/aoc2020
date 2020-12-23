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
	vector<uint64_t> mem(100000, 0);
	
	for (string line; getline(cin, line);) {
		smatch match;
		if (regex_match(line, match, regex {"mem\\[(\\d+)\\] = (\\d+)"})) {
			auto idx = stoi(match[1]);
			uint64_t val = stoi(match[2]);
			//while (mem.size() < idx) mem.push_back(0);
			mem[idx] = 0;
			
			printf("set %d to %lu\n", idx, val);
			printf("mem was: %lu\n", mem[idx]);
			for (uint64_t i = 0; i < 36; ++i) {
				if (mask[35-i] == 1) {
					mem[idx] |= (uint64_t) 1 << i;
				}
				else if (mask[35-i] == 0) {
					mem[idx] &= ~(1UL << i);
				}else {
					mem[idx] |= (val >> i & 1UL) << i;
				}
			}
			printf("mem is now: %lu\n", mem[idx]);
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
	for (uint64_t x : mem) {
		if (x > 0) cout << x << endl;
		s += x;
		if (s > ((uint64_t)1 << 60)) {
			printf("big!\n");
		}
	}
	cout << "P1: " << s << endl;

	mask = vector<int> (36, 0);
	mem = vector<uint64_t> (100000, 0);
}
