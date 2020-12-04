#include <cstdio>
#include <fstream>
#include <unordered_map>

using namespace std;

int main() {
  ifstream input("input.txt");

  int answer = 0;
  string line;

  int delim_count = 0;
  bool has_cid = false;
  while (getline(input, line)) {
    if (line == "") {
      if (delim_count == 8)
        answer++;
      if (delim_count == 7 && !has_cid)
        answer++;
      delim_count = 0;
      has_cid = false;
      continue;
    }

    delim_count += count(line.begin(), line.end(), ':');
    has_cid |= line.find("cid") != string::npos;
  }

  printf("answer: %d\n", answer);

  return 0;
}
