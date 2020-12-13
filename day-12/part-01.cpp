#include <cmath>
#include <cstdio>
#include <fstream>

using namespace std;

int main() {
  fstream input("input.txt");

  int x = 0, y = 0, theta = 0;

  string line;
  while (getline(input, line)) {
    char action = line[0];
    int value = stoi(line.substr(1));

    switch (action) {
    case 'N':
      y += value;
      break;
    case 'S':
      y -= value;
      break;
    case 'E':
      x += value;
      break;
    case 'W':
      x -= value;
      break;
    case 'L':
      theta = (theta + value) % 360;
      break;
    case 'R':
      theta = (theta + 360 - value) % 360;
      break;
    case 'F':
      if (theta == 0)
        x += value;
      else if (theta == 180)
        x -= value;
      else if (theta == 90)
        y += value;
      else if (theta == 270)
        y -= value;
    }
  }

  int distance = abs(x) + abs(y);

  printf("Distance: %d\n", distance);

  return 0;
}
