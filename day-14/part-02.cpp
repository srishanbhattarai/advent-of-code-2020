#include <bitset>
#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

using i64 = long long;

unordered_map<i64, i64> mem;

vector<i64> find_addrs_to_write(string mask, i64 addr) {
  bitset<36> addrbits(addr);
  string a = addrbits.to_string();

  vector<int> floating_bit_positions;
  for (int i = 0; i < mask.size(); ++i) {
    auto ch = mask[i];
    if (ch == '1')
      a[i] = ch;
    else if (ch == 'X') {
      a[i] = 'X';
      floating_bit_positions.push_back(i);
    }
  }

  vector<i64> addrs;
  int num_subsets = pow(2, floating_bit_positions.size());
  for (int i = 0; i < num_subsets; ++i) {
    string bits = bitset<36>(i).to_string();
    string addr(a);

    for (int j = 0; j < floating_bit_positions.size(); j++) {
      addr[floating_bit_positions[j]] = bits[36 - j - 1];
    }

    // reinterpret the bits back to a number
    addrs.push_back(bitset<36>(addr).to_ullong());
  }

  return addrs;
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

      vector<i64> addrs = find_addrs_to_write(mask, addr);
      for (auto addr : addrs)
        mem[addr] = value;
    }
  }

  i64 sum = 0;
  for (auto [k, v] : mem)
    sum += v;

  printf("Answer: %lld\n", sum);

  return 0;
}
