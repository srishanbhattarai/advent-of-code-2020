#include <fstream>
#include <cstdio>
#include <vector>

using namespace std;

int main() {
  ifstream input("input.txt");

  int num_lines = 323;

  vector<string> lines;
  lines.reserve(num_lines);

  string line;
  while (getline(input, line)) {
    lines.push_back(line);
  }

  int line_sz = lines[0].size();

  vector<pair<int, int>> check = {
    {1, 1},
    {3, 1},
    {5, 1},
    {7, 1},
    {1, 2}
  };

  int prod = 1;
  for (auto [i2, j2]: check) {
    int i = 0, j = 0, trees = 0;
    while (true) {
      i += i2;
      i = i % line_sz;
      j += j2;

      if (j > (num_lines - 1)) break;
      trees += lines[j][i] == '#';
    }

    prod *= trees;
  }

  printf("prod: %d\n", prod);

  return 0;
}
