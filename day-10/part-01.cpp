#include <cstdio>
#include <fstream>
#include <vector>

using namespace std;

int main() {
  fstream input("input.txt");

  vector<int> jolts;
  jolts.push_back(0);

  string line;
  while (getline(input, line))
    jolts.push_back(stoi(line));

  std::sort(jolts.begin(), jolts.end());
  jolts.push_back(jolts.back() + 3);

  int a = 0, b = 0;
  for (int i = 1; i < jolts.size(); ++i) {
    int diff = jolts[i] - jolts[i - 1];

    if (diff == 1)
      a++;
    else if (diff == 3)
      b++;
  }

  printf("%d\n", a * b);

  return 0;
}
