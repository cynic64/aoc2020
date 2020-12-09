#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

struct Seat {
	int row;
	int column;
};

int main() {
	std::vector<Seat> seats;
	
	for (std::string line; std::getline(std::cin, line); ) {
		if (line.size() != 10) throw std::runtime_error("Weird length");

		Seat seat {};
		for (int i = 0; i < 7; i++) seat.row += (line[i] == 'B') * 1 << (6 - i);
		for (int i = 0; i < 3; i++) seat.column += (line[7+i] == 'R') * 1 << (2 - i);

		seats.push_back(seat);
	}

	auto id = [](Seat seat){return seat.row * 8 + seat.column;};
	auto highest_seat = *std::max_element(seats.begin(), seats.end(),
						     [&](Seat a, Seat b) {
							     return id(a) < id(b);
						     });
	printf("Max id: %d\n", id(highest_seat));

	std::vector<std::vector<bool>> grid {1 << 3, std::vector<bool>(1 << 7, false)};

	for (auto seat : seats) grid[seat.column][seat.row] = true;

	for (auto line : grid) {
		for (auto seat : line) printf(seat? "#" : ".");
		printf("\n");
	}
}
