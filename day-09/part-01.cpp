#include <fstream>
#include <deque>
#include <unordered_set>

using namespace std;

const int PREAMBLE_SZ = 25;

void drop_add(deque<int>& deq, int el) {
  if (deq.size() == PREAMBLE_SZ) {
    deq.pop_back();
  }

  deq.push_front(el);
}

bool find_sum_pair(deque<int>& deq, int el) {
  unordered_set<int> comp;
  for (int i = 0; i < deq.size(); i++) {
    int key = el - deq[i];

    if (comp.count(key) != 0) {
      drop_add(deq, el);
      return true;
    }

    comp.insert(deq[i]);
  }

  drop_add(deq, el);
  return false;
}

int main() {
  fstream input("input.txt");

  deque<int> preamble;

  // read the preamble
  string line;
  for (int i = 0; i < PREAMBLE_SZ; i++) {
    getline(input, line);
    preamble.push_front(stoi(line));
  }

  // read rest of the lines
  while (getline(input, line)) {
    auto el = stoi(line);
    bool ok = find_sum_pair(preamble, el);

    if (!ok) {
      printf("Part one: %s\n", line.c_str());
      break;
    }
  }
}
