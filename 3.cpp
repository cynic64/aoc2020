#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

typedef std::vector<std::vector<bool>> BoolGrid;

class Forest {
public:
	Forest(BoolGrid grid) : grid(grid) {}

	long long check_slope(int right, int down);

	bool get(int x, int y);

	void print();

private:
	BoolGrid grid;
};

int main()
{
	BoolGrid grid;
	for (std::string line; std::getline(std::cin, line);) {
		std::vector<bool> row;
		for (auto c : line) row.push_back(c == '#' ? true : false);

		grid.push_back(row);
	}

	Forest forest {grid};

	forest.print();

	printf("Part 1: %lld\n", forest.check_slope(3, 1));
	printf("Part 2: %lld\n", forest.check_slope(1, 1)
	       * forest.check_slope(3, 1)
	       * forest.check_slope(5, 1)
	       * forest.check_slope(7, 1)
	       * forest.check_slope(1, 2));
}

long long Forest::check_slope(int right, int down)
{
	auto n = 0;
	auto x = right;

	for (auto y = down; y < grid.size(); y += down) {
		if (get(x, y)) n++;
		x += right;
	}

	return n;
}	

bool Forest::get(int x, int y)
{
	return grid[y % grid.size()][x % grid[0].size()];
}

void Forest::print()
{
	for (auto row : grid) {
		for (auto cell : row)
			std::cout << (cell ? "#" : ".") << " ";
		std::cout << std::endl;
	}
}
