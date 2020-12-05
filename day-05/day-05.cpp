#include <cstdio>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main() {
  ifstream input("input.txt");

  int max_seat = 0;
  int min_seat = 1000;
  int sum = 0;

  string line;
  while (getline(input, line)) {
    int lo = 0, hi = 127;
    int slo = 0, shi = 7;

    // row
    string seat = line.substr(0, 7);
    for (char fb: seat) { // front or back 
      auto mid = (lo + hi)/2;
      if (fb == 'F') hi = mid;
      else if (fb == 'B') lo = mid + 1;
    }

    // col
    auto seat_col = line.substr(7);
    for (char lr: seat_col) {
      auto mid = (slo + shi)/2;
      if (lr == 'L') shi = mid;
      else if (lr == 'R') slo = mid + 1;
    }

    // calcs
    auto row = line[6] == 'F' ? lo : hi;
    auto col = line[line.size() - 1] == 'L' ? slo : shi;
    int value = row * 8 + col;

    sum += value;
    max_seat = max(max_seat, value);
    min_seat = min(min_seat, value);
  }

  min_seat -= 1;
  int missing_range_sum = min_seat * (min_seat + 1) / 2;
  int expected_sum = (max_seat * (max_seat + 1) / 2) - missing_range_sum;

  printf("Maximum: %d\n", max_seat);
  printf("Mine: %d\n", expected_sum - sum);

  return 0;
}
