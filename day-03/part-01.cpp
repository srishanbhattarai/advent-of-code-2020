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

  int i = 0, j = 0, trees = 0;
  while (true) {
    i += 3;
    i = i % line_sz;

    j += 1;
    if (j > (num_lines - 1)) break;

    trees += lines[j][i] == '#';
  }

  printf("Trees: %d\n", trees);

  return 0;
}
