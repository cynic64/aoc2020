#include <bits/stdc++.h>

using namespace std;

int main() {
	unordered_map<string, set<string>> poss;
	for (string line; getline(cin, line);) {
		istringstream iss(line);
		set<string> ingr;
		set<string> al;
		string token;
		while (iss >> token && token != "(contains") ingr.insert(token);
		while (iss >> token) al.insert(token.substr(0, token.size()-1));
		for (auto a : al) {
			if (poss.find(a) == poss.end()) poss[a] = ingr;
			else {
				auto old = poss[a];
				poss[a].clear();
				set_intersection(old.begin(), old.end(), ingr.begin(), ingr.end(), inserter(poss[a], poss[a].end()));
			}
		}
	}

	for (auto [al, ingr] : poss) {
		cout << al << endl;
		for (auto i : ingr) cout << "\t" << i << endl;
	}

	set<string> taken;
	unordered_map<string, string> which;
	while (taken.size() < poss.size()) {
		for (auto [al, ingr] : poss) {
			set<string> diff;
			set_difference(ingr.begin(), ingr.end(), taken.begin(), taken.end(), inserter(diff, diff.begin()));
			if (diff.size() == 1) {
				taken.insert(*diff.begin());
				which[al] = *diff.begin();
			}
		}
	}

	vector<string> als;
	for (auto [al, ingr] : which) als.push_back(al);
	sort(als.begin(), als.end());
	for (auto a : als) cout << which[a] << ",";
	cout << endl;
}
	
