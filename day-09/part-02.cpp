#include <fstream>
#include <deque>
#include <unordered_set>
#include <vector>
#include <cstdint>

using namespace std;

using i64 = int64_t;

pair<i64, i64> find_range_with_sum(vector<i64>& numbers, i64 sum) {
  deque<i64> deq;

  i64 curr_sum = 0;
  int i = 0;

  while (curr_sum != sum) {
    if (curr_sum < sum) {
      deq.push_front(numbers[i]);
      curr_sum += numbers[i];

      i++;
   } else {
      int back = deq.back();
      deq.pop_back();
      curr_sum -= back;
    }
  }

  i64 curr_min = deq[0], curr_max = deq[0];
  for (int i = 1; i < deq.size(); ++i) {
    curr_min = min(curr_min, deq[i]);
    curr_max = max(curr_max, deq[i]);
  }

  return {curr_min, curr_max};
}

int main() {
  fstream input("input.txt");

  vector<i64> numbers;
  string line;
  while (getline(input, line)) numbers.push_back(strtoll(line.c_str(), NULL, 10));
  
  const i64 part_one_answer = 105950735;
  auto [lo, hi] = find_range_with_sum(numbers, part_one_answer);

  printf("Part two answer: [%lld + %lld = %lld]\n", lo, hi, lo + hi);
}
