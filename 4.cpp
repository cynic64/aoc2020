#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>

int main()
{
	std::string cur_pass;
	auto p1 = 0;
	auto p2 = 0;

	for (;;) {
		std::string line;
		std::getline(std::cin, line);

		if (line.size() > 0) {
			cur_pass.append(line);
			continue;
		}

		auto exists = [=](std::string s){return cur_pass.find(s) != std::string::npos;};
		p1 += exists("byr:") && exists("iyr:") && exists("eyr")
			&& exists("hgt") && exists("hcl") && exists("ecl") && exists("pid");

		p2 += std::regex_search(cur_pass, std::regex {"byr:(19[2-9][0-9]|200[0-2])"})
			&& std::regex_search(cur_pass, std::regex {"iyr:(201[0-9]|2020)"})
			&& std::regex_search(cur_pass, std::regex {"eyr:(202[0-9]|2030)"})
			&& std::regex_search(cur_pass, std::regex {"hgt:(
		
		cur_pass = "";
		
		if (std::cin.eof()) break;
	}

	printf("Part 1: %d\n", p1);
	printf("Part 2: %d\n", p2);
}
