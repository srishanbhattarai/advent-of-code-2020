#include <bitset>
#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

using i64 = long long;

unordered_map<i64, i64> mem;

i64 apply_mask(string mask, i64 value) {
  bitset<36> bits(value);
  string v = bits.to_string();

  for (int i = 0; i < mask.size(); ++i) {
    auto ch = mask[i];
    if (ch != 'X')
      v[i] = ch;
  }

  i64 masked_val = bitset<36>(v).to_ullong();

  return masked_val;
}

int main() {
  fstream input("input.txt");

  string mask = "";

  string line;
  while (getline(input, line)) {
    // parse mask
    if (line.find("mask") != string::npos) {
      mask = line.substr(7);
    } else {
      i64 marker = line.find("=");
      i64 addr = strtoll(line.substr(4, marker - 2).c_str(), NULL, 10);
      i64 value = strtoll(line.substr(marker + 2).c_str(), NULL, 10);

      mem[addr] = apply_mask(mask, value);
    }
  }

  i64 sum = 0;
  for (auto [k, v] : mem)
    sum += v;

  printf("Answer: %lld\n", sum);

  return 0;
}
