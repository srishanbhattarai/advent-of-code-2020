#include <cmath>
#include <cstdio>
#include <fstream>

using namespace std;

pair<int, int> rotate_left(int px, int py, int theta) {
  int t = theta % 360;
  int x = px, y = py;

  for (int i = 0; i < t / 90; ++i) {
    swap(x, y);
    x *= -1;
  }

  return {x, y};
}

int main() {
  fstream input("input.txt");

  int x = 0, y = 0;
  int wpx = 10, wpy = 1;

  string line;
  while (getline(input, line)) {
    printf("%s %d %d %d %d\n", line.c_str(), x, y, wpx, wpy);
    char action = line[0];
    int value = stoi(line.substr(1));

    switch (action) {
    case 'N':
      wpy += value;
      break;
    case 'S':
      wpy -= value;
      break;
    case 'E':
      wpx += value;
      break;
    case 'W':
      wpx -= value;
      break;
    case 'L': {
      auto [lx, ly] = rotate_left(wpx, wpy, value);
      wpx = lx;
      wpy = ly;
      break;
    }
    case 'R': {
      auto [rx, ry] = rotate_left(wpx, wpy, 360 - value);
      wpx = rx;
      wpy = ry;
      break;
    }
    case 'F':
      x += wpx * value;
      y += wpy * value;
    }
  }

  int distance = abs(x) + abs(y);

  printf("Distance: %d\n", distance);

  return 0;
}
