#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
	std::vector<int> numbers;
	for (std::string line; std::getline(std::cin, line);)
		numbers.push_back(std::stoi(line));

	for (auto i : numbers) {
		auto res = std::find_if(numbers.begin(), numbers.end(),
					[=](int n) { return n + i == 2020; });

		if (res != numbers.end())
			printf("%d * %d = %d\n", i, *res, i * *res);
	}

	for (auto i : numbers)
		for (auto j : numbers)
			for (auto k : numbers)
				if (i + j + k == 2020)
					printf("%d * %d * %d = %d\n",
					       i, j, k, i*j*k);
}
