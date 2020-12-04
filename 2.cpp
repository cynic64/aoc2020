#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

int main() {
	auto part1 = 0;
	auto part2 = 0;
	for (std::string line; std::getline(std::cin, line);) {
		auto pos = line.find("-");
		auto range_min = std::stoi(line.substr(0, pos));

		auto last = pos+1;
		pos = line.find(" ", last);
		auto range_max = std::stoi(line.substr(last, (pos-last)));
		
		last = pos+1;
		pos = line.find(" ", pos+1);
		auto chr = line.substr(last, 1)[0];
		auto pass = line.substr(pos+1);

		auto n = std::count(pass.begin(), pass.end(), chr);
		if (n >= range_min && n <= range_max) part1++;
		if ((pass[range_min-1] == chr && pass[range_max-1] != chr)
		    || (pass[range_min-1] != chr && pass[range_max-1] == chr)) part2++;
	}

	printf("Part 1: %d\n", part1);
	printf("Part 2: %d\n", part2);
}

