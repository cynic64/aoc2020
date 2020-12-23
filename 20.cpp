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
#include <cmath>
#include <array>

using namespace std;

void print_row(vector<bool> row) {
	for (auto cell : row) cout << (cell ? "# " : ". ");
	cout << endl;
}

struct Tile {
	int id;
	vector<vector<bool>> cells;

	bool operator==(const Tile &rhs) const {
		return rhs.id == id;
	}

	vector<vector<bool>> get_edges() {
		vector<bool> top = cells[0];
		vector<bool> bottom = cells[cells.size() - 1];
		vector<bool> left;
		vector<bool> right;

		for (size_t y = 0; y < cells.size(); ++y) {
			left.push_back(cells[y][0]);
			right.push_back(cells[y][cells[0].size() - 1]);
		}

		return {top, left, bottom, right};
	}

	void print() {
		printf("ID: %d\n", id);
		for (auto row : cells) {
			print_row(row);
		}
		cout << "----------" << endl;
		auto edges = get_edges();
		for (auto e : edges) {
			print_row(e);
		}
		cout << endl;
	}

	Tile de_edge() {
		Tile t;
		t.id = id;

		for (size_t y = 1; y < cells.size() - 1; ++y) {
			vector<bool> row;
			for (size_t x = 1; x < cells[0].size() - 1; ++x) row.push_back(cells[y][x]);
			t.cells.push_back(row);
		}

		return t;
	}

	Tile rotate() {
		// CCW
		Tile t;
		t.id = id;
		t.cells = vector<vector<bool>>(cells.size(), vector<bool>(cells[0].size(), false));

		int r = cells.size();
		int c = cells[0].size();
		int my = r / 2, mx = c / 2;

		for (auto y = 0; y < r; ++y) {
			for (auto x = 0; x < c; ++x) {
				int y2 = y - my, x2 = x - mx;
				swap(x2, y2);
				x2 *= -1;
				y2 += my;
				x2 += mx;
				t.cells[y2][x2-1] = cells[y][x];
			}
		}

		return t;
	}

	Tile flip() {
		Tile t;
		t.cells = vector<vector<bool>>(cells.size(), vector<bool>(cells[0].size(), false));
		t.id = id;
		// Across y=x

		for (size_t y = 0; y < cells.size(); ++y) {
			for (size_t x = 0; x < cells[0].size(); ++x) {
				t.cells[x][y] = cells[y][x];
			}
		}

		return t;
	}
};

int main() {
	vector<Tile> tiles;
	Tile cur_tile;
	for (string line; getline(cin, line);) {
		if (line.size() == 0) {
			tiles.push_back(cur_tile);
			cur_tile = {};
		} else if (line.find("Tile") != string::npos) {
			cur_tile.id = stoi(line.substr(5));
		} else {
			cur_tile.cells.push_back({});
			for (auto c : line) cur_tile.cells.back().push_back(c == '#' ? true : false);
		}
	}
	if (cur_tile.cells.size()) tiles.push_back(cur_tile);

	// Edge --> list of ids with that edge
	unordered_map<vector<bool>, vector<int>> edge_map;

	for (auto& t : tiles) {
		t.print();
		auto edges = t.get_edges();
		for (auto e : edges) {
			edge_map[e].push_back(t.id);
			reverse(e.begin(), e.end());
			edge_map[e].push_back(t.id);
		}
	}

	for (auto [edge, ids] : edge_map) {
		print_row(edge);
		printf("  ^---- Occurs with IDs: ");
		for (auto i : ids) printf("%d ", i);
		printf(".\n\n");
	}

	vector<Tile> corners;
	vector<Tile> edge_tiles;
	vector<Tile> middle_tiles;
	for (auto t : tiles) {
		auto unmatched_ct = 0;
		auto edges = t.get_edges();
		for (auto e : edges) if (edge_map[e].size() == 1) unmatched_ct++;

		cout << "ID " << t.id << " has " << unmatched_ct << " unmatched edges" << endl;

		if (unmatched_ct == 2) corners.push_back(t);
		else if (unmatched_ct == 1) edge_tiles.push_back(t);
		else if (unmatched_ct == 0) middle_tiles.push_back(t);
		else {
			printf("??? %d unmatched\n", unmatched_ct);
			throw;
		}
	}

	cout << "Corners:" << endl;
	for (auto t : corners) cout << t.id << endl;

	cout << edge_tiles.size() << " edges" << endl;
	cout << middle_tiles.size() << " middle tiles" << endl;

	uint64_t product = 1;
	for (auto t : corners) product *= t.id;
	cout << "P1: " << product << endl << endl << endl;

	auto world_size = (int) sqrt(tiles.size());
	cout << "World size: " << world_size << endl;
	
	vector<vector<Tile>> world(world_size, vector<Tile>(world_size, {0, {}}));

	vector<array<int, 2>> offsets = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

	//corners[0] = corners[0].flip();

	for (auto y = 0; y < world_size; ++y) {
		for (auto x = 0; x < world_size; ++x) {
			printf("\n\n");
			printf("X: %d, Y: %d\n", x, y);
			printf("--------------WORLD:--------------\n");
			for (auto y = 0; y < world_size; ++y) {
				for (auto x = 0; x < world_size; ++x) {
					cout << world[y][x].id << ' ';
				}
				cout << endl;
			}
			for (auto cy = 0; cy < world_size * 10; ++cy) {
				for (auto cx = 0; cx < world_size * 10; ++cx) {
					auto ty = cy / 10;
					auto tx = cx / 10;
					auto sy = cy - ty * 10;
					auto sx = cx - tx * 10;
					auto cell = false;
					if (world[ty][tx].id) cell = world[ty][tx].cells[sy][sx];
					cout << (cell ? "# " : ". ");
				}
				cout << endl;
			}
			printf("----------------------------------\n");
			vector<Tile*> has_to_match;
			for (auto [xo, yo] : offsets) {
				auto nx = x + xo, ny = y + yo;
				if (ny >= world_size || nx >= world_size || ny < 0 || nx < 0) has_to_match.push_back(nullptr);
				else if (world[ny][nx].id) has_to_match.push_back(&world[ny][nx]);
				else has_to_match.push_back(nullptr);
			}
			printf("Has to match:\n");
			for (auto t : has_to_match) {
				if (t) printf("\t%d\n", t->id);
				else printf("\tnull\n");
			}

			Tile chosen {0, {}};

			// Special case : top-left corner
			if (x == 0 && y == 0) chosen = corners[0];

			else {
				// Otherwise just find something that fits
				vector<Tile> all_tiles;
				if ((x == 0 && y == 0) || (x == world_size-1 && y == 0) || (x == 0 && y == world_size-1) || (x == world_size-1 && y == world_size-1)) all_tiles = corners;
				else if (x == 0 || y == 0 || x == world_size-1 || y == world_size-1) all_tiles = edge_tiles;
				else all_tiles = middle_tiles;
				printf("%lu tiles to choose from\n", all_tiles.size());
				while (all_tiles.size() > 1) {
					vector<Tile> tiles_to_delete;
					for (auto t : all_tiles) {
						for (auto i = 0; i < 4; ++i) {
							// top (0) --> bottom (2), left (1) -->
							// right (3), bottom (2) --> top, etc.
							if (!has_to_match[i]) continue;
							auto edge = has_to_match[i]->get_edges()[(i+2) % 4];
							auto m = edge_map[edge];
							if (find(m.begin(), m.end(), t.id) == m.end()) {
								printf("\tTile with id %d does not work.\n", t.id);
								printf("\tUnmatcheable edge:\n");
								print_row(edge);
								tiles_to_delete.push_back(t);
								break;
							}
						}
					}
					printf("Deleteing %lu tiles\n", tiles_to_delete.size());
					for (auto t: tiles_to_delete) {
						if (find(all_tiles.begin(), all_tiles.end(), t) != all_tiles.end()) all_tiles.erase(find(all_tiles.begin(), all_tiles.end(), t));
					}
					printf("%lu left:", all_tiles.size());
					for (auto t : all_tiles) cout << ' ' << t.id;
					cout << endl;
				}

				chosen = all_tiles[0];
			}

			if (!chosen.id) {
				printf("Did not choose anything!\n");
				throw;
			}

			printf("Chosen ID: %d\n", chosen.id);

			// Any unmatcheable edges have to be on the rim
			vector<size_t> unmatched_indices;
			if (y == 0) unmatched_indices.push_back(0);  // top
			else if (y == world_size - 1) unmatched_indices.push_back(2); // bottom
			if (x == 0) unmatched_indices.push_back(1); // left
			else if (x == world_size - 1) unmatched_indices.push_back(3); // right

			printf("Unmatched indices:");
			for (auto i : unmatched_indices) cout << ' '<< i;
			cout << endl;

			// Rotate and flip it until it works
			auto attempts = 0;
			for (;;) {
				auto ok = true;
				for (auto i : unmatched_indices) {
					if (edge_map[chosen.get_edges()[i]].size() != 1) ok = false;
				}

				for (auto i = 0; i < 4; ++i) {
					if (!has_to_match[i]) continue;
					auto edge_to_match = has_to_match[i]->get_edges()[(i+2) % 4];
					auto our_edge = chosen.get_edges()[i];
					if (edge_to_match != our_edge) ok = false;
				}

				if (ok) break;

				if (attempts % 3 == 0) chosen = chosen.rotate();
				else chosen = chosen.flip();
				attempts += 1;
			}

			printf("Found an orientation after %d attempts:\n", attempts);
			chosen.print();

			world[y][x] = chosen;

			// Delete 
			auto corner_pos = find(corners.begin(), corners.end(), chosen);
			auto edge_pos = find(edge_tiles.begin(), edge_tiles.end(), chosen);
			auto middle_pos = find(middle_tiles.begin(), middle_tiles.end(), chosen);
			if (corner_pos != corners.end()) corners.erase(corner_pos);
			else if (edge_pos != edge_tiles.end()) edge_tiles.erase(edge_pos);
			else if (middle_pos != middle_tiles.end()) middle_tiles.erase(middle_pos);
			else throw;

			auto deleted = 0;
			for (auto [edge, ids] : edge_map) {
				auto id_pos = find(ids.begin(), ids.end(), chosen.id);
				if (id_pos != ids.end()) {
					ids.erase(id_pos);
					deleted++;
				}
			}

			printf("Deleted %d edge entries\n", deleted);
		}
	}

	for (auto y = 0; y < world_size; ++y) {
		for (auto x = 0; x < world_size; ++x) {
			world[y][x] = world[y][x].de_edge();
		}
	}

	vector<vector<bool>> merged (world_size * 8, vector<bool>(world_size * 8));
	for (auto cy = 0; cy < world_size * 8; ++cy) {
		for (auto cx = 0; cx < world_size * 8; ++cx) {
			auto ty = cy / 8;
			auto tx = cx / 8;
			auto sy = cy - ty * 8;
			auto sx = cx - tx * 8;
			auto cell = false;
			if (world[ty][tx].id) cell = world[ty][tx].cells[sy][sx];
			merged[cy][cx] = cell;
		}
		cout << endl;
	}

	vector<string> pattern;
	pattern.push_back("                  # ");
	pattern.push_back("#    ##    ##    ###");
	pattern.push_back(" #  #  #  #  #  #   ");

	auto pw = pattern[0].size(), ph = pattern.size();

	printf("--------------WORLD:--------------\n");
	for (auto y = 0; y < world_size; ++y) {
		for (auto x = 0; x < world_size; ++x) {
			cout << world[y][x].id << ' ';
		}
		cout << endl;
	}
	for (auto r : merged) print_row(r);
	printf("----------------------------------\n");

	vector<int> occs;
	for (auto i = 0; i < 100; ++i) {
		auto occ = 0;
		vector<array<int, 2>> poss;
		auto mh = merged.size(), mw = merged[0].size();
		for (auto y = 0; y < mh-ph; ++y) {
			for (auto x = 0; x < mw-pw; ++x) {
				auto works = true;
				for (auto py = 0; py < ph; ++py) {
					for (auto px = 0; px < pw; ++px) {
						if (pattern[py][px] == '#' && merged[y+py][x+px] == false) {
							//printf("Fail at (%d, %d)\n", px, py);
							//printf("p: %c, M: %c\n", pattern[py][px], merged[y+py][x+px]);
							works = false;
							break;
						}
					}
				}
				if (works) {
					poss.push_back({x, y});
					occ++;
				}
			}
		}

		printf("POSS\n");
		for (auto [x, y]: poss) printf("\t%d %d\n", x, y);

		auto copy = merged;
		for (auto [x, y]: poss) {
			for (auto py = 0; py < ph; ++py) {
				for (auto px = 0; px < pw; ++px) {
					auto yy = y + py;
					auto xx = x + px;
					if (yy >= mh || x >= mw) continue;
					if (pattern[py][px] != '#') continue;

					copy[yy][xx] = false;
				}
			}
		}

		auto z = 0;
		for (auto r : copy) for (auto x : r) if (x) z++;

		cout << occ <<", " << z << endl;
		occs.push_back(occ);

		vector<vector<bool>> new_merged(mh, vector<bool>(mw));
					
		if (i % 3 == 0) {
			int r = merged.size();
			int c = merged[0].size();
			int my = r / 2, mx = c / 2;

			for (auto y = 0; y < r; ++y) {
				for (auto x = 0; x < c; ++x) {
					int y2 = y - my, x2 = x - mx;
					swap(x2, y2);
					x2 *= -1;
					y2 += my;
					x2 += mx;
					new_merged[y2][x2-1] = merged[y][x];
				}
			}
		} else {
			for (auto y = 0; y < mh; ++y) {
				for (auto x = 0; x < mw; ++x) {
					new_merged[x][y] = merged[y][x];
				}
			}
		}

		merged = new_merged;
	}

	cout << *max_element(occs.begin(), occs.end()) << endl;
}
