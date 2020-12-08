#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// forward decl
vector<string> split(string text, string delim);

// state
int next_id = 0;                      // next_id to assign to bag name
unordered_map<string, int> bag_names; // maps ids to bag names

// For an index i, this adjacency lists gives you all the nodes with an incoming
// edge to i
vector<vector<pair<int, int>>> adjacency_inward;

// For an index i, this adjacency lists gives you all the nodes that can be
// reached from i (outward)
vector<vector<pair<int, int>>> adjacency_outward;

// looks up bag and gets it's id, otherwise assigns a new one.
int get_or_assign_id(string name) {
  if (bag_names.count(name) == 0) {
    adjacency_inward.resize(next_id + 1);
    adjacency_outward.resize(next_id + 1);
    bag_names[name] = next_id;

    next_id += 1;

    return next_id - 1;
  }

  return bag_names[name];
}

// DFS on the inward list
int find_answer_part_one(int curr_node, unordered_set<int> &visited) {
  vector<pair<int, int>> next = adjacency_inward[curr_node];
  int sum = 0;

  for (auto [neighbor, weight] : next) {
    if (visited.count(neighbor) != 0)
      continue;

    visited.insert(neighbor);
    sum += 1 + find_answer_part_one(neighbor, visited);
  }

  return sum;
}

// DFS on the outward adjacency and calculates the number of bags as instructed
// in the problem.
int find_answer_part_two(int curr_node) {
  vector<pair<int, int>> next = adjacency_outward[curr_node];
  int sum = 0;

  for (auto [neighbor, weight] : next) {
    sum += weight + weight * find_answer_part_two(neighbor);
  }

  return sum;
}

int main() {
  fstream input("input.txt");

  string line;
  // walks through each line, and creates a dependency graph
  while (getline(input, line)) {
    vector<string> words = split(line, " ");

    // the current bag is denoted by first two words
    string node = words[0] + words[1];
    int node_id = get_or_assign_id(node);

    // string parsing: keep finding the word 'bags', and then backtrack 2 words
    // i = 3 because the first 3 words indicate current bag, we want to find
    // dependencies which are further down the string.
    for (int i = 3; i < words.size(); i++) {
      auto word = words[i];
      auto is_bags = word.find("bag") != string::npos;
      if (is_bags) {
        if (words[i - 2] == "no")
          continue; // edge case
        int weight = stoi(words[i - 3]);
        int bag_id = get_or_assign_id(words[i - 2] + words[i - 1]);

        adjacency_inward[bag_id].push_back({node_id, weight});
        adjacency_outward[node_id].push_back({bag_id, weight});
      }
    }
  }

  // the thing we are looking for.
  string target = "shinygold";
  int target_id = get_or_assign_id(target);

  unordered_set<int> visited;
  int answer_one = find_answer_part_one(target_id, visited);
  int answer_two = find_answer_part_two(target_id);

  printf("Part one: %d\n", answer_one);
  printf("Part two: %d\n", answer_two);

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
