#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// self explanatory
bool is_num(string str) {
  bool ok = true;
  for (auto ch : str)
    ok &= isdigit(ch);
  return ok;
}

// self explanatory
bool bounded(string str, int lo, int hi) {
  auto num = stoi(str);

  return num >= lo && num <= hi;
}

// finds the chunk corresponding to a single key starting at start_pos on that line.
string chunk(string line, int start_pos) {
  int value_start_pos = start_pos + 4;

  string value = "";

  for (auto ch : line.substr(value_start_pos)) {
    if (ch == ' ' || ch == '\n') {
      break;
    }

    value += ch;
  }

  return value;
}

// finds all the keys in the line, and then returns the value of each of those in that line
pair<vector<string>, bool> find_kvs(string line, vector<string> keys) {
  bool found_all = true;
  vector<int> positions;
  for (string k : keys) {
    auto pos = line.find(k);
    positions.push_back(pos);
    found_all &= pos != string::npos;
  }

  if (!found_all) {
    return {{}, false};
  }

  vector<string> kvs;
  for (int pos : positions) {
    auto value = chunk(line, pos);
    kvs.push_back(value);
  }

  return {kvs, true};
}

// concatenates passports split across multiple lines to be across a single line
vector<string> get_passports(string str) {
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

// height rules given in the problem statement
bool height_ok(string value) {
  char last = value[value.size() - 1];
  string val = value.substr(0, value.size() - 2);
  if (last == 'm') {
    return is_num(val) && bounded(val, 150, 193);
  } else if (last == 'n') {
    return is_num(val) && bounded(val, 59, 76);
  } else {
    return false;
  }
}

// self explanatory
bool contains_color(string str) {
  vector<string> colors = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
  for (auto ch : colors) {
    if (str.find(ch) != string::npos)
      return true;
  }

  return false;
}

int main() {
  ifstream input("input.txt");
  stringstream buffer;
  buffer << input.rdbuf();

  vector<string> pps = get_passports(buffer.str());

  int valid = 0;

  vector<string> to_find = {
      "byr", // 0
      "iyr", // 1
      "eyr", // 2
      "hgt", // 3
      "hcl", // 4
      "ecl", // 5
      "pid"  // 6
  };

  for (string pp : pps) {
    auto [kvs, found] = find_kvs(pp, to_find);
    if (!found)
      continue;

    valid += (is_num(kvs[0]) && bounded(kvs[0], 1920, 2002)) &&
             (is_num(kvs[1]) && bounded(kvs[1], 2010, 2020)) &&
             (is_num(kvs[2]) && bounded(kvs[2], 2020, 2030)) &&
             height_ok(kvs[3]) && (kvs[4][0] == '#' && kvs[4].size() == 7) &&
             contains_color(kvs[5]) && (is_num(kvs[6]) && kvs[6].size() == 9);
  }

  printf("Valid: %d\n", valid);

  return 0;
}
