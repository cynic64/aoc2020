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

struct Event {
	uint64_t every;
	uint64_t starting;
};

void print(Event e) {
	cout << "Every: " << e.every << ", starting: " << e.starting << endl;
}

uint64_t gcd(uint64_t a, uint64_t b) {
	if (b < a) swap(a, b);

	if (a == 0) return b;

	return gcd(b%a, a);
}
	
uint64_t lcm(uint64_t a, uint64_t b) {
	return a*(b/gcd(a, b));
}

Event combine(Event a, Event b) {
	// Bigger event goes in b
	if (a.every > b.every) swap(a, b);

	Event combined;
	combined.every = lcm(a.every, b.every);

	for (uint64_t i = b.starting; ; i += b.every) {
		// We know i works with a, so just check with b
		if (i % a.every == a.starting && i % b.every == b.starting) {
			combined.starting = i;
			return combined;
		}
	}
}

int main() {
	string line;
	getline(cin, line);
	auto min_time = stoi(line);

	vector<pair<int, int>> ids;
	auto idx = 0;
	while (getline(cin, line, ',')) {
		if (line[0] != 'x') {
			cout << idx << endl;
			ids.push_back({stoi(line), idx});
		}
		++idx;
	}

	auto best_id = -1;
	auto best_time = 99999999;
	for (auto [id, idx]: ids) {
		auto time = id * (min_time / id + 1);
		auto diff = time - min_time;
		if (diff < best_time) {
			best_id = id;
			best_time = diff;
		}
	}

	cout << "P1: " << best_id*best_time << endl;

	vector<Event> events;
	for (auto [id, idx] : ids) {
		int rem = (id-idx);
		if (rem == id) rem = 0;
		while (rem < 0) rem += id;
		events.push_back({(uint64_t) id, (uint64_t) rem});
	}

	for (auto ev : events) print(ev);

	Event a {7, 0};
	Event b {13, 12};
	auto c = combine(a, b);
	Event e {1, 0};
	for (auto ev : events) {
		print(e);
		e = combine(e, ev);
	}
	cout << "Final:" << endl;
	print(e);
}
