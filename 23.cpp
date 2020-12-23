#include <bits/stdc++.h>

using namespace std;

int main() {
	//string in = "389125467";
	string in = "362981754";
	vector<int> num;
	for (auto c : in) num.push_back(c-'0');
	auto min = *min_element(num.begin(), num.end());
	auto max = *max_element(num.begin(), num.end());
	while (num.size() < 1'000'000) num.push_back(++max);
	
	auto N = num.size();

	// Cup # -> index of next
	vector<size_t> cups(max+1);
	vector<bool> removed(max+1);
	for (size_t i = 0; i < N; ++i) cups[num[i]] = num[(i+1)%N];

	auto cur = num[0];
	for (auto i = 0; i < 10'000'000; ++i) {
		auto a = cups[cur], b = cups[a], c = cups[b];
		removed[a] = true;
		removed[b] = true;
		removed[c] = true;
		cups[cur] = cups[c];
		auto dest = cur - 1;
		while (dest < min || removed[dest]) {
			dest--;
			if (dest < min) dest = max;
		}
		auto after_dest = cups[dest];
		cups[dest] = a;
		cups[c] = after_dest;
		removed[a] = false;
		removed[b] = false;
		removed[c] = false;
		cur = cups[cur];
	}

	cout << cups[1] * cups[cups[1]] << endl;
}
