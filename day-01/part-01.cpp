/**
 * Maintains a hash set of each number seen so far and query it for each number
 * using the number's complement. We are guaranteed to find a solution so this
 * works in O(n) time.
 */

#include <cstdio>
#include <fstream>
#include <unordered_set>

std::pair<int, bool> process(std::unordered_set<int> &seen, int num,
                             int target) {
  auto match = seen.find(target);
  if (match == seen.end()) {
    seen.insert(num);
    return {0, false};
  }

  return {target, true};
}

int main() {
  std::ifstream input("input.txt");
  std::unordered_set<int> seen;

  int num;
  while (input >> num) {
    auto [match, success] = process(seen, num, 2020 - num);
    if (success) {
      printf("%d + %d = %d\n", match, num, match + num);
      printf("%d * %d = %d\n", match, num, match * num);
      break;
    }
  }
}
