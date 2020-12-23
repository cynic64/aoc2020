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

using namespace std;

uint64_t eval(string exp) {
	cout << "Called with: " << exp << endl;
	// Just a number
	smatch match;
	if (regex_match(exp, match, regex("(\\d+)"))) {
		auto a = stoul(match[1]);
		printf("Just a number: %lu\n", a);
		return a;
	}

	// Nothing
	if (exp.size() == 0) return 0;

	// Brackets
	if (exp.find('(') != string::npos) {
		auto start = exp.find('(');
		auto last = start+1;
		auto end = exp.find(')', start+1);
		while (exp.find('(', last) != string::npos && exp.find('(', last) < end) {
			last = end+1;
			end = exp.find(')', last);
		}

		auto pre = exp.substr(0, start);
		auto subexp = to_string(eval(exp.substr(start+1, end-start-1)));
		auto post = exp.substr(end+1);

		printf("Pre: '%s', subexp: '%s', post: '%s'\n", pre.c_str(), subexp.c_str(), post.c_str());

		pre.append(subexp);
		pre.append(post);
		printf("Combined: '%s'\n", pre.c_str());

		return eval(pre);
	}

	// No brackets, but some addition
	if (regex_match(exp, match, regex("(.*?)(\\d+) [+] (\\d+)(.*)"))) {
		auto a = stoul(match[2]);
		auto b = stoul(match[3]);
		auto n = to_string(a+b);
		string m = match[1];
		cout << "add, m1: " << m << ", a: " << a << ", b: " << b << ", n: " << n << ", r: " << match[4] << "|\n";
		m.append(n); m.append(match[4]);

		return eval(m);
	}

	// Only multiplication
	if (regex_match(exp, match, regex("(\\d+) [*] (\\d+)(.*)"))) {
		auto a = eval(match[1]);
		auto b = stoul(match[2]);
		string n = to_string(a*b);
		n.append(match[3]);

		return eval(n);
	}

	else {
		printf("???\n");
		return -1;
	}
}

int main() {
	uint64_t s = 0;
	for (string line; getline(cin, line);) {
		auto x = eval(line);
		cout << line << ": " << x << endl;
		s += x;
	}
	cout << s << endl;
}
