#include <cstdint>
#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

using i64 = int64_t;

/**
 * Explanation of how this works:
 *
 * Imagine a DAG created from the sorted data such that the maximum value is the
 * source of the DAG. Each vertex in the DAG represents an adapter, and has
 * outward edges to only those vertices that are compatible with this adapter.
 *
 * From the constraints of the question, we know that maximum outdegree(x) for
 * any x is 3. So, there may be up to 3 combinations from 3 adjacent vertices.
 * These adjacent vertices will have their own number of combinations from their
 * neighbors (up to 3 again). For any x, these numbers are (x - 1), (x - 2) and
 * (x - 3). They may not all exist in the data, so we check for it first.
 *
 * The graph is acyclic since numbers monotonically decrease so combination is
 * counted twice.
 *
 * We keep recursing down to base cases, until we hit zero (which must be a sink
 * of the DAG), or we hit <0 (which means the graph has been exhausted).
 *
 * Since the same vertices will be adjacent to a multitude of other vertices,
 * use dynamic programming to store the intermediate results.
 */

i64 solve(i64 curr_joltage, unordered_set<i64> &joltages,
          unordered_map<i64, i64> &dp) {
  if (joltages.count(curr_joltage) == 0) {
    dp[curr_joltage] = 0;
    return 0;
  }

  if (dp.count(curr_joltage) != 0)
    return dp[curr_joltage];

  if (curr_joltage <= 0) {
    auto val = curr_joltage == 0 ? 1 : 0;
    dp[curr_joltage] = val;
    return val;
  }

  for (auto valid_joltage_diff : {1, 2, 3})
    dp[curr_joltage] += solve(curr_joltage - valid_joltage_diff, joltages, dp);

  return dp[curr_joltage];
}

int main() {
  fstream input("input.txt");
  string line;

  unordered_set<i64> jolts;
  jolts.insert(0);

  i64 jmax = 0;
  while (getline(input, line)) {
    jmax = max(jmax, strtoll(line.c_str(), NULL, 10));
    jolts.insert(stoi(line));
  }
  jolts.insert(jmax + 3);

  unordered_map<i64, i64> dp;
  printf("%lld\n", solve(jmax, jolts, dp));

  return 0;
}
