#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>
#include <cassert>

using namespace std;

int main() {
	vector<string> lines;
	for (string line; getline(cin, line); ) {
		line.insert(line.begin(), '.');
		line.insert(line.end(), '.');
		lines.push_back(line);
	}

	string s1 (lines[0].size(), '.');
	string s2 (lines[0].size(), '.');
	lines.insert(lines.begin(), s1);
	lines.insert(lines.end(), s1);

	for (auto line : lines) cout << line << endl;

	vector<string> next_gen {};

	while (next_gen != lines) {
		if (next_gen.size() > 0) lines = next_gen;
		if (next_gen.size() == 0) next_gen = {lines};

		/*
		for (int y = 1; y < lines.size() - 1; ++y) {
			for (int x = 1; x < lines[0].size() - 1; ++x) {
				auto is_occ = [&](int x, int y){ return lines[y][x] == '#' ? 1 : 0; };
				auto is_empty = [&](int x, int y){ return lines[y][x] == 'L' ? 1 : 0; };
				auto occ = 0;
				auto em = 0;
				for (auto q = -1; q < 2; ++q) {
					for (auto p = -1; p < 2; ++p) {
						if (p == 0 && q == 0) continue;
						occ += is_occ(x+p, y+q);
						em += is_empty(x+p, y+q);
					}
				}

				auto cur_occ = is_occ(x, y);
				auto cur_em = is_empty(x, y);

				if (cur_em && occ == 0) next_gen[y][x] = '#';
				else if (cur_occ && occ >= 4) next_gen[y][x] = 'L';
			}
		}
		*/

		for (int y = 1; y < lines.size() - 1; ++y) {
			for (int x = 1; x < lines[0]2.size() - 1; ++x) {
				vector<vector<int>> offsets = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
				vector<int> found (8, -1);
				for (auto dist = 1; dist < lines.size(); ++dist) {
					auto found_count = 0;
					for (auto i = 0; i < offsets.size(); ++i) {
						if (found[i] != -1) {
							++found_count;
							continue;
						}
						auto off = offsets[i];
						auto nx = x + dist * off[0];

						if (nx < 1 || nx >= lines[0].size() - 1) {
							found[i] = 0;
							continue;
						}

						auto ny = y + dist * off[1];
						if (ny < 1 || ny >= lines.size() - 1) {
							found[i] = 0;
							continue;
						}

						if (lines[ny][nx] == '#') found[i] = 1;
						if (lines[ny][nx] == 'L') found[i] = 0;
					}
					if (found_count == found.size()) break;
				}
				auto ncount = 0;
				for (auto f : found) if (f == 1) ncount++;

				// if (lines[y][x] == '.') cout << '.';
				// else if (lines[y][x] == '#' || lines[y][x] == 'L') cout << ncount;

				if (lines[y][x] == '.') continue;
				if (lines[y][x] == 'L' && ncount == 0) next_gen[y][x] = '#';
				else if (lines[y][x] == '#' && ncount >= 5) next_gen[y][x] = 'L';
			}
			// cout << endl;
		}

		for (auto line : next_gen) cout << line << endl;
	}

	uint64_t count = 0;
	for (auto line : next_gen) count += std::count(line.begin(), line.end(), '#');
	cout << count << endl;
}
