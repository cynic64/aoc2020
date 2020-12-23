#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <unordered_map>
#include <cassert>
#include <cmath>
#include <numeric>
#include <variant>
#include <string_view>

using namespace std;

typedef vector<size_t> Option;
typedef vector<Option> Options;
typedef variant<Options, char> Rule;

// Returns a vector mapping indices to whether a match up to that index is
// possible with the given rule or not.
vector<bool> matches(string_view s, size_t r_idx, unordered_map<size_t, Rule> rules) {
	if (auto c = get_if<char>(&rules[r_idx])) {
		// No match of length 0, but one of length 1
		if (s[0] == *c) return {false, true};
		return {};
	} else if (auto options = get_if<Options>(&rules[r_idx])) {
		vector<bool> combined (s.size()+1, false);

		for (auto option : *options) {
			vector<bool> prev(s.size()+1, false);
			prev[0] = true;
			vector<bool> next(s.size()+1, false);
			for (auto option_r_idx : option) {
				for (size_t start = 0; start < s.size(); ++start) {
					if (!prev[start]) continue;
					
					auto slice = s.substr(start);
					auto m = matches(slice, option_r_idx, rules);
					for (size_t length = 0; length < m.size(); ++length) {
						if (m[length]) next[start+length] = true;
					}
				}

				swap(next, prev);
				fill(next.begin(), next.end(), false);
			}

			for (size_t length = 0; length < prev.size(); ++length)
				if (prev[length]) combined[length] = true;
		}

		return combined;
	}
	printf("??? r_idx: %lu\n", r_idx);
	throw;
}

int main() {
	vector<string> msgs;
	unordered_map<size_t, Rule> rules;
	for (string line; getline(cin, line) && line.size();) {
		cout << line << endl;

		vector<string> tokens;
		istringstream iss(line);
		copy(istream_iterator<string>(iss),
		     istream_iterator<string>(),
		     back_inserter(tokens));

		auto idx = stoul(tokens[0]);

		if (tokens[1][0] == '"') {
			rules[idx] = tokens[1][1];
			continue;
		}

		Options options {{}};
		for (auto t = tokens.begin() + 1; t != tokens.end(); ++t) {
			if (*t == "|") options.push_back({});
			else options.back().push_back(stoi(*t));
		}
		rules[idx] = options;
	}

	for (string line; getline(cin, line);) msgs.push_back(line);

	cout << "Rules:" << endl;
	for (auto [idx, rule] : rules) {
		if (auto c = get_if<char>(&rule)) printf("idx: %lu, c: '%c'\n", idx, *c);
		else if (auto options = get_if<Options>(&rule)) {
			cout << "idx: " << idx << ", options: ";
			for (auto o : *options) {
				cout << '(';
				for (auto i : o) cout << i << ' ';
				cout << "), ";
			}
			cout << endl;
		} else throw;
	}

	auto c = 0;
	for (auto m : msgs) {
		auto match_lengths = matches(m, 0, rules);
		if (match_lengths[m.size()]) {
			cout << "Matches: " << m << endl;
			c++;
		} else {
			cout << "Does not match: " << m << endl;
		}

		cout << "Match lengths:" << endl;
		for (auto l : match_lengths) cout << l << ' ';
		cout << endl << endl;
	}
	cout << endl << c << endl;
}
