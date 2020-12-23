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
	vector<vector<vector<vector<bool>>>> grid (50, vector<vector<vector<bool>>>(50, vector<vector<bool>>(50, vector<bool>(50, false))));
	auto y = 20;
	for (string line; getline(cin, line); ++y) {
		for (auto x = 0; x < line.size(); ++x) {
			grid[20][20][x+20][y] = line[x] == '#';
		}
	}

	auto next = grid;
	for (auto i = 0; i < 6; ++i) {
		for (auto z = 1; z < 49; ++z) {
			for (auto y = 1; y < 49; ++y) {
				for (auto x = 1; x < 49; ++x) {
					for (auto w = 1; w < 49; ++w) {
						auto nc = 0;
						for (auto zo = -1; zo < 2; ++zo) {
							for (auto yo = -1; yo < 2; ++yo) {
								for (auto xo = -1; xo < 2; ++xo) {
									for (auto wo = -1; wo < 2; ++wo) {
										if (xo == 0 && yo == 0 && zo == 0 && wo == 0) continue;
										nc += grid[z+zo][y+yo][x+xo][w+wo] ? 1 : 0;
									}
								}
							}
						}

						auto cur = grid[z][y][x][w];
						if (nc == 3) next[z][y][x][w] = true;
						else if (nc == 2 && cur) next[z][y][x][w] = true;
						else next[z][y][x][w] = false;
					}
				}
			}
		}

		grid = next;

		auto s = 0;
		for (auto i : grid) {
			for (auto j : i) {
				for (auto k : j) {
					for (auto p : k) s += p ? 1 : 0;
				}
			}
		}

		cout << s << endl;

	}
}









