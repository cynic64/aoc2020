#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>

int main()
{
	std::vector<std::string> answers;

	auto p1 = 0;
	auto p2 = 0;

	for (;;) {
		std::string line;
		std::getline(std::cin, line);

		if (line.size() > 0) {
			answers.push_back(line);
			continue;
		}

		std::vector<char> already;
		for (auto answer : answers)
			for (auto c : answer)
				if (std::find(already.begin(), already.end(), c) == already.end())
					already.push_back(c);
		p1 += already.size();

		std::unordered_map<char, int> all;
		for (auto answer : answers)
			for (auto c : answer)
				all[c]++;

		for (auto pair : all)
			if (pair.second == answers.size()) p2++;
		
		if (std::cin.eof()) break;
		answers = {};
	}

	printf("Part 1: %d\n", p1);
	printf("Part 2: %d\n", p2);
}
