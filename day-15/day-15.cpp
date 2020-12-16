#include <unordered_map>
#include <vector>

// Compiled with -O3 and just waited for it to finish running.
// ¯\_(ツ)_/¯

using namespace std;

unordered_map<int, pair<int, int>> mem;
unordered_map<int, int> spoken_count;

int main() {
  vector<int> input = {0, 20, 7, 16, 1, 18, 15};

  vector<int> spoken;
  // handle starting numbers
  int index = 1;
  for (auto s : input) {
    mem[s] = {0, index++};
    spoken.push_back(s);
    spoken_count[s]++;
  }

  const int until = 30000000; // 2000 for p1
  while (index <= until) {
    int curr = spoken.back();

    if (spoken_count[curr] == 1) {
      // curr was spoken only once before
      spoken.push_back(0);
      spoken_count[0]++;
      auto [a, b] = mem[0];
      mem[0] = {b, index};
    } else {
      // spoken_count[curr] can never be 0, so it must be > 1 here
      auto [a, b] = mem[curr];
      int speak = b - a;
      spoken.push_back(speak);
      spoken_count[speak]++;

      auto [p, q] = mem[speak];
      mem[speak] = {q, index};
    }

    index++;
  }

  printf("Answer: %d\n", spoken.back());

  return 0;
}
