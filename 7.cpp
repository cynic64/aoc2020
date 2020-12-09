#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>
#include <cassert>

std::unordered_map<std::string,
		   std::unordered_map<std::string, int>> map;

std::unordered_map<std::string, int> decompose(std::string color)
{
	if (map[color].size() == 0) return {};

	std::unordered_map<std::string, int> out;
	
	for (auto sub_color : map[color]) {
		out[sub_color.first] += sub_color.second;
		for (auto c : decompose(sub_color.first))
			out[c.first] += c.second * sub_color.second;
	}

	return out;
}

int main()
{
	for (std::string line; std::getline(std::cin, line); ) {
		if (line.find("contain no other bags") != std::string::npos) {
			auto pos = line.find(" contain");
			auto key = line.substr(0, pos);
			map[key] = {};
		} else {
			auto pos = line.find(" bags");
			auto key = line.substr(0, pos);
			map[key] = {};

			auto last = line.find("contain ") + 8;
			for (;;) {
				pos = line.find(", ", last);
				auto k = line.substr(last, (pos-last));
				auto n = std::stoi(k);
				auto j = k.substr(2, k.find(" bag")-2);
				map[key][j] = n;
				last = pos+2;

				if (pos == std::string::npos) break;
			}
		}
	}

	auto p1 = 0;
	for (auto c: map) {
		auto comp = decompose(c.first);
		printf("%s:\n", c.first.c_str());
		for (auto x : comp) printf("\t%s: %d\n", x.first.c_str(), x.second);
		if (comp.find("shiny gold") != comp.end()) p1++;
	}

	auto p2 = 0;
	for (auto c: decompose("shiny gold")) p2 += c.second;

	printf("Part 1: %d\n", p1);
	printf("Part 2: %d\n", p2);
}
