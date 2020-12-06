#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// concatenates answers split across multiple lines to be across a single line,
// separated by spaces.
vector<string> get_answers_for_group(string str) {
  vector<string> results;
  while (str.size()) {
    int index = str.find("\n\n");

    if (index == string::npos) {
      // end of file, push remaining
      results.push_back(str);
      str = "";
    } else {
      results.push_back(str.substr(0, index));
      str = str.substr(index + 2); // \n\n are two chars
      if (str.size() == 0)
        results.push_back(str); // rest of the file
    }
  }

  return results;
}

int main() {
  ifstream input("input.txt");
  stringstream buffer;
  buffer << input.rdbuf();

  int part_one_sum = 0;
  int part_two_sum = 0;

  vector<string> answers_for_group = get_answers_for_group(buffer.str());

  for (string ans : answers_for_group) {
    unordered_set<char> chars;

    // for each char, true if it has been seen in every answer
    unordered_map<char, bool> seen;

    for (auto ch: ans) {
      if (ch == '\n') continue;

      chars.insert(ch);

      if (seen.count(ch) == 0) seen[ch] = true;
      else seen[ch] &= true;
    }

    for (auto it = seen.begin(); it != seen.end(); ++it) {
      if (it->second)  part_two_sum++;
    }

    part_one_sum += chars.size();
  }

  printf("Part one: %d\n", part_one_sum);
  printf("Part two: %d\n", part_two_sum);

  return 0;
}
