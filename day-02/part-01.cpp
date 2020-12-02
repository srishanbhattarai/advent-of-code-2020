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

    int count = std::count(password.begin(), password.end(), letter);
    bool ok = count >= lo && count <= hi;
    if (ok)
      valid++;
  }

  printf("%d valid passwords\n", valid);

  return 0;
}
