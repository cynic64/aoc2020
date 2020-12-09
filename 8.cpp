#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>
#include <cassert>

// Returns whether or not the program terminated, and the value of the
// accumulator.
std::pair<bool, int> exec(std::vector<std::pair<std::string, int>> code)
{
	auto idx = 0;
	auto acc = 0;
	std::vector<int> idxs;

	while (std::find(idxs.begin(), idxs.end(), idx) == idxs.end() && idx < code.size()) {
		auto p = code[idx];
		auto ins = p.first;
		auto arg = p.second;
		idxs.push_back(idx);
		// std::cout << idx << ": " << ins << " | " << arg << std::endl;
		if (ins == "acc") acc += arg;
		else if (ins == "jmp") {
			idx += arg;
			continue;
		}
		else assert(ins == "nop");
		idx++;
	}

	auto terminated = idx == code.size();

	return {terminated, acc};
}

int main()
{
	std::vector<std::pair<std::string, int>> code;
	for (std::string line; std::getline(std::cin, line); ) {
		std::smatch match;
		std::regex_match(line, match, std::regex { "(...) (.\\d+)" });
		assert(match.size() == 3);
		code.push_back({match[1], std::stoi(match[2])});
	}

	auto p1 = exec(code).second;
	auto p2 = 0;
	for (int i = 0; i < code.size(); i++) {
		auto copy = code;
		if (copy[i].first == "jmp")
			copy[i].first = "nop";
		else if (copy[i].first == "nop")
			copy[i].first = "jmp";
		else continue;
		
		if (exec(copy).first == true) {
			p2 = exec(copy).second;
			break;
		}
	}

	printf("Part 1: %d\n", p1);
	printf("Part 2: %d\n", p2);
}
