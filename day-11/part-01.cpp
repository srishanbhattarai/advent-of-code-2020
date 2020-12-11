#include <fstream>
#include <vector>

using namespace std;

int find_num_occupied_adjacent(vector<string>& grid, int row, int col) {
  vector<pair<int, int>> position_delta = {
      {-1, 0},  // top
      {-1, -1}, // top left
      {-1, 1},  // top right

      {0, -1}, // left
      {0, 1},  // left

      {1, 0},  // bot
      {1, -1}, // bot left
      {1, 1},  // bot right
  };

  int adj = 0;
  for (auto [xd, yd] : position_delta) {
    // avoid bounds check by consuming the exception
    try {
      char ch = grid.at(row + xd).at(col + yd);
      if (ch == '#')
        adj++;
    } catch (...) {
      // ignore
    }
  }

  return adj;
}

int main() {
  fstream input("input.txt");

  vector<string> lines;
  string line;
  while (getline(input, line))
    lines.push_back(line);

  vector<string> next(lines);

  int num_occupied;
  while (true) {
    num_occupied = 0;

    for (int i = 0; i < lines.size(); i++) {
      auto line = lines[i];

      for (int j = 0; j < line.size(); j++) {
        char seat = line[j];

        if (seat == 'L') {
          int n = find_num_occupied_adjacent(lines, i, j);

          if (n == 0) {
            num_occupied++;
            next[i][j] = '#';
          }
        } else if (seat == '#') {
          num_occupied++;
          int n = find_num_occupied_adjacent(lines, i, j);

          if (n >= 4) {
            next[i][j] = 'L';
            num_occupied--;
          }
        }
      }
    }

    if (lines == next)
      break;
    else
      lines = next;
  }

  printf("Occupied: %d\n", num_occupied);
}
