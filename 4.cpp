#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>

int main()
{
	std::vector<std::string> req_fields = {"byr:","iyr:", "eyr:", "hgt:", "hcl:", "ecl:", "pid:"};

	std::string cur_pass;
	auto p1 = 0;
	auto p2 = 0;
	
	for (std::string line; std::getline(std::cin, line);) {
		if (line.size() == 0) {
			auto ok_p1 = true;
			for (auto f : req_fields)
				if (cur_pass.find(f) == std::string::npos) {
					ok_p1 = false;
					break;
				}
			if (ok_p1) p1++;

			auto ok_p2 = true;
			for (auto f : req_fields) {
				auto pos = cur_pass.find(f);
				if (pos == std::string::npos) {
					ok_p2 = false;
					break;
				}

				auto val_end = cur_pass.find(" ", pos+4);
				auto val = cur_pass.substr(pos+4, val_end-(pos+4));
				if (f == "byr:") {
					auto y = std::stoi(val);
					ok_p2 &= y >= 1920 && y <= 2002;
				} else if (f == "iyr:") {
					auto y = std::stoi(val);
					ok_p2 &= y >= 2010 && y <= 2020;
				} else if (f == "eyr:") {
					auto y = std::stoi(val);
					ok_p2 &= y >= 2020 && y <= 2030;
				} else if (f == "hgt:") {
					std::smatch match;
					std::regex regex {"([0-9]+)(in|cm)"};
					if (std::regex_search(val, match, regex) == false) {
						ok_p2 = false;
					} else {
						if (match.size() == 3) {
							auto h = std::stoi(match[1]);
							ok_p2 &= (match[2] == "cm" && h >= 150 && h <= 193)
								|| (match[2] == "in" && h >= 59 && h <= 76);
						} else ok_p2 = false;
					}
				} else if (f == "hcl:") {
					if (!(std::regex_match(val, std::regex {"#[0-9a-f]{6}"})))
						ok_p2 = false;
				} else if (f == "ecl:") {
					ok_p2 &= std::regex_match(val, std::regex("(amb|blu|brn|gry|grn|hzl|oth)"));
				} else if (f == "pid:") {
					ok_p2 &= std::regex_match(val, std::regex("[0-9]{9}"));
				}
			}

			if (ok_p2) p2++;

			cur_pass = "";
		} else {
			cur_pass.append(line);
			cur_pass.append(" ");
		}
	}

	printf("Part 1: %d\n", p1);
	printf("Part 2: %d\n", p2);
}
