#include <iostream>
#include <vector>
#include <string>
#include <complex>

using namespace std;

int main() {
	vector<pair<char, int>> lines;
	for (string line; getline(cin, line);) {
		auto dir = line[0];
		auto n = stoi(line.substr(1));
		lines.push_back({dir, n});
	}

	// Part 1
	complex<double> s(0, 0), I(0, 1);
	complex<double> t[127];
	t['N'] = {0, 1};
	t['E'] = {1, 0};
	t['S'] = {0, -1};
	t['W'] = {-1, 0};
	t['F'] = {1, 0};

	for (auto [dir, n] : lines) {
		if (dir == 'L') t['F'] *= pow(I, n / 90.0);
		else if (dir == 'R') t['F'] *= pow(-I, n / 90.0);
		else s += t[dir] * (double) n;
	}

	cout << "Part 1: " << abs(s.real()) + abs(s.imag()) << endl;

	// Part 2
	t['F'] = {10, 1};
	s = {0, 0};

	for (auto [dir, n] : lines) {
		if (dir == 'L') t['F'] *= pow(I, n / 90.0);
		else if (dir == 'R') t['F'] *= pow(-I, n / 90.0);
		else if (dir == 'F') s += t['F'] * (double) n;
		else t['F'] += t[dir] * (double) n;
	}

	cout << "Part 2: " << abs(s.real()) + abs(s.imag()) << endl;
}
