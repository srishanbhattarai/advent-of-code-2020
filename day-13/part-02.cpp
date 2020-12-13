#include <cstdint>
#include <cstdio>
#include <vector>

using namespace std;

using i64 = int64_t;

const string input =
    "19,x,x,x,x,x,x,x,x,41,x,x,x,37,x,x,x,x,x,787,x,x,x,x,x,x,x,x,x,x,x,x,13,x,"
    "x,x,x,x,x,x,x,x,23,x,x,x,x,x,29,x,571,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,17";

// does what it says - can people add string.split to STL please.
vector<string> split(string str, string delim) {
  vector<string> results;
  while (str.size()) {
    int index = str.find(delim);

    if (index == string::npos) {
      // end of file, push remaining
      results.push_back(str);
      str = "";
    } else {
      results.push_back(str.substr(0, index));
      str = str.substr(index + delim.size());
      if (str.size() == 0)
        results.push_back(str); // rest of the file
    }
  }

  return results;
}

// apparently, you're supposed to do this using Chinese Remainder Theorem.
//
// The goal is to find a 'n' such that for each index i
//  (n + i) is perfectly divided by input[i]
//
// This program outputs a Wolfram Alpha compatible equation to represent this
// problem, so just copy paste whatever this outputs into Wolfram Alpha, and be
// done with this stupid question.
int main() {
  vector<string> sp = split(input, ",");
  for (int i = 0; i < sp.size(); i++) {
    if (sp[i] != "x") {
      auto id = strtoll(sp[i].c_str(), NULL, 10);
      printf("(n+%d)%%%lld = 0", i, id);
      if (i != sp.size() - 1)
        printf(",");
    }
  }
  printf("\nPaste that into https://www.wolframalpha.com/\n");
}
