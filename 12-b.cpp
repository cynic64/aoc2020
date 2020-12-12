#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>
#include <cassert>
#include <cmath>

using namespace std;

int main() {
	vector<pair<char, int>> lines;
	for (string line; getline(cin, line);) {
		auto dir = line[0];
		auto n = stoi(line.substr(1));
		lines.push_back({dir, n});
	}

	auto x = 0, y = 0, dx = 1, dy = 0;
	for (auto [dir, n] : lines) {
		if (dir == 'N') y += n;
		else if (dir == 'E') x += n;
		else if (dir == 'S') y -= n;
		else if (dir == 'W') x -= n;
		else if (dir == 'F') {
			x += n * dx;
			y += n * dy;
		} else if (dir == 'L') {
			for (; n >= 90; n -= 90) {
				swap(dx, dy);
				dx *= -1;
			}
		} else if (dir == 'R') {
			for (; n >= 90; n -= 90) {
				swap(dx, dy);
				dy *= -1;
			}
		}
	}
	cout << "Part 1: " << abs(x) + abs(y) << endl;

	x = 0, y = 0;
	auto vx = 10, vy = 1;
	for (auto [dir, n] : lines) {
		if (dir == 'N') vy += n;
		else if (dir == 'E') vx += n;
		else if (dir == 'S') vy -= n;
		else if (dir == 'W') vx -= n;
		else if (dir == 'F') {
			x += n * vx;
			y += n * vy;
		} else if (dir == 'L') {
			for (; n >= 90; n -= 90) {
				swap(vx, vy);
				vx *= -1;
			}
		} else if (dir == 'R') {
			for (; n >= 90; n -= 90) {
				swap(vx, vy);
				vy *= -1;
			}
		}
	}

	cout << "Part 2: " << abs(x) + abs(y) << endl;
}
