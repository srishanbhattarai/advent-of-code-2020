#include <fstream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

vector<string> split(string str, string delim);
string join(vector<string> str, string delim);

// first half of the input
unordered_map<string, string> rules;

// 2nd half of the input
vector<string> strings;

unordered_map<string, string> dp;

string regexify_rule(string rule_num, bool part_two) {
  if (dp.find(rule_num) != dp.end()) {
    return dp[rule_num];
  }

  // part two specific section.
  if (part_two) {
    // 8: 42 | 42 8
    //
    // the first rule is a simple recursion so the regex pattern for it
    // would be regex(42)+ (1 or more times)
    if (rule_num == "8") {
      dp["8"] = regexify_rule("42", part_two) + "+";

      return dp["8"];
    }

    // 11: 42 31 | 42 11 31
    // Expanding the inner 11 in the second rule:
    //  (regex(42) regex(42){n} regex(31){n} regex(31)
    //
    // Or,
    //  regex(42){n + 1}regex(31){n + 1}
    //
    // this is tricker because we don't know how many repititons of 'n' will
    // match all the valid options in the problem. So we pick a number like 50,
    // which hopefully does the job.
    //
    // This took me a while to figure out and I don't know if c++ regexes
    // are slow/bad but the Python version of this runs for n=100 and executes
    // instantly (check sophiebits' code on github) which turns out is pretty
    // much the same as this one, minus the dynamic programming (which doesn't
    // matter because the bulk of the time is spent in regex matching which is
    // mega slow here).
    //
    // This code takes a good while to run even with -O3, and if you set n too
    // high (> 50) it may not even complete.
    //
    // I frequently got this error:
    //    The complexity of an attempted match against a regular expression
    //    exceeded a pre-set level.
    // which is "documented" here:
    // http://www.cplusplus.com/reference/regex/regex_error/
    if (rule_num == "11") {
      auto first = regexify_rule("42", part_two);
      auto second = regexify_rule("31", part_two);

      vector<string> patterns;
      for (int n = 1; n < 40; n++) {
        patterns.push_back(first + "{" + std::to_string(n) + "}" + second +
                           "{" + std::to_string(n) + "}");
      }

      dp["11"] = "(" + join(patterns, "|") + ")";
      return dp["11"];
    }
  }

  auto rule = rules[rule_num];
  // only "a" and "b" contain a double quote
  if (rule.find("\"") != string::npos) {
    // For a rule "a", rule[1] == a
    return string(1, rule[1]);
  }

  // There are two other types of rules:
  // 80: 131 97
  // and, 131: 123 97 | 97 123
  //
  // We want a regex that recursively evaluates each of the
  // possibilities and joins them using a "|"
  auto possibilities = split(rule, " | ");
  vector<string> patterns;
  for (auto p : possibilities) { // ["123 97", "97 123"]
    vector<string> intermediate;

    for (auto r : split(p, " ")) { // "[123, 97]"
      auto pattern = regexify_rule(r, part_two);
      intermediate.push_back(pattern);
    }

    // rules within the same side of the "|" have to be concatenated
    patterns.push_back(join(intermediate, ""));
  }

  // all the patterns must be separated by an "|" since they are to be matched
  // using an "OR"
  dp[rule_num] = "(" + join(patterns, "|") + ")";
  return dp[rule_num];
}

int solve(string god_regex) {
  // for each string, check if it matches this god pattern
  int sum = 0;
  for (auto str : strings)
    sum += std::regex_match(str, std::regex(god_regex));

  return sum;
}

int main() {
  fstream in("input.txt");
  stringstream buffer;
  buffer << in.rdbuf();

  auto input = split(buffer.str(), "\n\n");

  // split into strings  l lj
  auto first_half = input[0];
  auto second_half = input[1];

  // parse the rules
  auto first_half_parts = split(first_half, "\n");
  for (string rule : first_half_parts) {
    vector<string> data = split(rule, ": ");
    rules[data[0]] = data[1];
  }

  // parse the strings
  auto second_half_parts = split(second_half, "\n");
  for (auto str : second_half_parts) {
    if (str != "")
      strings.push_back(str);
  }

  // the idea is to create a god regex that matches all strings
  // that satisfy rule 0
  auto zero = string(1, '0');
  printf("Part One Answer: %d\n", solve(regexify_rule(zero, false)));
  dp.clear();
  printf("Part Two Answer: %d\n", solve(regexify_rule(zero, true)));

  return 0;
}

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

string join(vector<string> v, string delim) {
  string res;
  for (int i = 0; i < v.size(); i++) {
    res += v[i];
    if (i != v.size() - 1)
      res += delim;
  }

  return res;
}
