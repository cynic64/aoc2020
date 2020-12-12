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
	vector<pair<string, int>> lines;
	for (string line; getline(cin, line); ) {
		smatch match;
		if (regex_match(line, match, regex {"(.)(\\d+)"})) {
			lines.push_back({match[1], stoi(match[2])});
		} else {
			cout << "eek!" << endl;
		}
	}

	double dx = 1;
	double dy = 0;
	double dd = 90;
	double x = 0, y = 0;
	for (auto line : lines) {
		cout << line.first << " " << line.second << endl;
		if (line.first == "N") y -= line.second;
		if (line.first == "S") y += line.second;
		if (line.first == "E") x += line.second;
		if (line.first == "W") x -= line.second;
		if (line.first == "F") {
			x += line.second * dx;
			y += line.second * dy;
		}
		if (line.first == "R") {
			dd = fmod((dd + line.second), 360.0);
			dx = sin(dd * M_PI / 180.0);
			dy = -cos(dd * M_PI / 180.0);
		}
		if (line.first == "L") {
			dd = (dd - line.second);
			if ( dd < 0) dd += 360.0;
			dx = sin(dd * M_PI / 180.0);
			dy = -cos(dd * M_PI / 180.0);
		}
		//printf("    x: %d, y: %d, dx: %d, dy: %d
	}

	cout << "p1: " << x << " " << y << " " << abs(x)+abs(y) << endl;

	double sx = 0, sy = 0;
	double wx = 10, wy = -1;
	// printf("    sx: %f, sy: %f, wx: %f, wy: %f\n", sx, sy, wx, wy);

	for (auto line : lines) {
		// cout << line.first << " " << line.second << endl;
		if (line.first == "N") wy -= line.second;
		if (line.first == "S") wy += line.second;
		if (line.first == "E") wx += line.second;
		if (line.first == "W") wx -= line.second;
		if (line.first == "F") {
			double dx = (wx-sx)*(double)line.second;
			double dy = (wy-sy)*(double)line.second;
			sx += dx;
			sy += dy;
			wx += dx;
			wy += dy;
		}
		if (line.first == "R") {
			double dy = wy-sy;
			double dx = wx-sx;
			double dist = sqrt(dx*dx+dy*dy);
			double theta = atan2(dx, dy) * 180 / M_PI;
			double ntheta = theta - line.second;
			while (ntheta < 0) ntheta += 360;
			ntheta = fmod(ntheta, 360.0);

			double nx = sin(ntheta * M_PI / 180.0) * dist;
			double ny = cos(ntheta * M_PI / 180.0) * dist;
			wy = sy+ny;
			wx = sx+nx;
		}
		if (line.first == "L") {
			double dy = wy-sy;
			double dx = wx-sx;
			double dist = sqrt(dx*dx+dy*dy);
			double theta = atan2(dx, dy) * 180 / M_PI;
			double ntheta = theta + line.second;
			while (ntheta < 0) ntheta += 360;
			ntheta = fmod(ntheta, 360.0);

			double nx = sin(ntheta * M_PI / 180.0) * dist;
			double ny = cos(ntheta * M_PI / 180.0) * dist;
			wy = sy+ny;
			wx = sx+nx;
		}
		// printf("    sx: %f, sy: %f, wx: %f, wy: %f\n", sx, sy, wx, wy);
	}

	printf("sx: %f, sy: %f, wx: %f, wy: %f\n", sx, sy, wx, wy);
	cout << abs(sy) + abs(sx) << endl;
}
