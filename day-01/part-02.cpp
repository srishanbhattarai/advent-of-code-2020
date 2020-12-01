/**
 * We fix each number as the key in each iteration, and then try to find two
 * other numbers that sum to (2020 - key) as we did in part 01.
 *
 * Time complexity: O(n*n)
 */

#include <array>
#include <cstdio>
#include <fstream>
#include <unordered_set>
#include <vector>

struct Solution {
  std::array<int, 3> numbers;
  bool success;
};

// copied from part 01
std::pair<int, bool> find_sum(std::unordered_set<int> &seen, int num,
                              int target) {
  auto match = seen.find(target);
  if (match == seen.end()) {
    seen.insert(num);
    return {0, false};
  }

  return {target, true};
}

Solution find_solution(const std::vector<int> &numbers) {
  for (int first : numbers) {
    auto target = 2020 - first;

    std::unordered_set<int> seen;
    for (int second : numbers) {
      if (first == second)
        continue;

      auto [third, found] = find_sum(seen, second, target - second);
      if (found) {
        return Solution{{first, second, third}, true};
      }
    }
  }

  return Solution{{0, 0, 0}, false};
}

int main() {
  std::ifstream input("input.txt");

  std::vector<int> numbers;
  numbers.reserve(200);

  int num;
  while (input >> num) {
    numbers.push_back(num);
  }

  Solution sol = find_solution(numbers);
  if (!sol.success) {
    printf("No solution found\n");
  } else {
    int sum = 0, prod = 1;
    for (int n : sol.numbers) {
      sum += n;
      prod *= n;
    }

    printf("%d + %d + %d = %d\n", sol.numbers[0], sol.numbers[1],
           sol.numbers[2], sum);
    printf("%d * %d * %d = %d\n", sol.numbers[0], sol.numbers[1],
           sol.numbers[2], prod);
  }
}
