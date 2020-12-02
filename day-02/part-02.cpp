#include <cstdio>
#include <fstream>
#include <sstream>

int main() {
  std::ifstream input("input.txt");

  int valid = 0;

  char _x, letter;
  int lo, hi;
  std::string password;

  std::string line;
  while (std::getline(input, line)) {
    std::stringstream lstream(line);
    lstream >> lo >> _x >> hi >> letter >> _x >> password;

    bool first = password[lo - 1] == letter;
    bool second = password[hi - 1] == letter;

    valid += first ^ second;
  }

  printf("%d Valid passwords\n", valid);

  return 0;
}
