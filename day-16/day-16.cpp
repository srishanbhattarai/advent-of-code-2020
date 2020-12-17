#include <fstream>
#include <unordered_set>
#include <vector>

using namespace std;

using i64 = long long;

vector<string> split(string str, string delim) {
  vector<string> results;
  while (str.size()) {
    i64 index = str.find(delim);

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

struct Options {
  i64 min_1;
  i64 max_1;
  i64 min_2;
  i64 max_2;
};

// Rules
vector<string> rules;
vector<Options> min_max;

// Tickets
vector<i64> myticket;
vector<vector<i64>> nearby_tickets;

// Parses a single rule line
void parse_rule(string line) {
  auto rulesp = split(line, ":");
  rules.push_back(rulesp[0]);

  auto minmaxsp = split(rulesp[1], "or");

  Options options;
  i64 i = 0;
  for (string entry : minmaxsp) {
    auto mm = split(entry, "-");
    if (i == 0) {
      options.min_1 = strtoll(mm[0].c_str(), NULL, 10);
      options.max_1 = strtoll(mm[1].c_str(), NULL, 10);
    } else if (i == 1) {
      options.min_2 = strtoll(mm[0].c_str(), NULL, 10);
      options.max_2 = strtoll(mm[1].c_str(), NULL, 10);
    }
    i++;
  }

  min_max.push_back(options);
}

bool check_rule(i64 rule_id, i64 entry) {
  Options o = min_max[rule_id];

  return (entry >= o.min_1 && entry <= o.max_1) ||
         (entry >= o.min_2 && entry <= o.max_2);
}

// check every entry in the nearby tickets
// against the ranges for every rule and sum up the invalid entries.
pair<unordered_set<i64>, i64> solve_part_one() {
  unordered_set<i64> wrong;
  i64 sum = 0;

  // for each nearby ticket
  i64 row = 0;
  for (vector<i64> nbt : nearby_tickets) {
    // for each number in the ticket
    for (i64 i = 0; i < nbt.size(); ++i) {
      auto entry = nbt[i];
      bool entry_ok = false;

      // check against each field
      for (i64 j = 0; j < min_max.size(); ++j) {
        entry_ok = check_rule(j, entry);
        if (entry_ok)
          break;
      }

      if (!entry_ok) {
        wrong.insert(row);
        sum += entry;
      }
    }

    row++;
  }

  return {wrong, sum};
}

/**
 * For each field, go through all the entries in a single column of the nearby
 * entries.
 *
 * If all of them satisfy the field's rules, then that column is a candidate for
 * the field. There may be several columns that match a single field so we
 * cannot immediately assign the column to that field.
 *
 * If we find a field that has a single candidate, then we assign it. This
 * column (candidate) can no longer be assigned to any other field. We repeat
 * until all of them are assigned.
 */
i64 solve_part_two(unordered_set<i64> wrong_ticket_idxs) {
  const i64 num_cols = myticket.size();
  const i64 num_rows =
      nearby_tickets.size() + 1; // +1 because we add our ticket manually
  const i64 num_fields = rules.size();

  nearby_tickets.push_back(myticket);

  // For any index 'i' the ordering[i] represents the field that corresponds
  // to the column 'i'.
  vector<i64> ordering(num_fields);
  fill(ordering.begin(), ordering.end(), -1);

  // keeps track of how many fields have been definitively ordered.
  // it is the same as the number of elements in ordering that are not equal to
  // -1.
  int num_ordered_fields = 0;

  while (true) {
    for (i64 i = 0; i < num_fields; i++) {

      // all columns that are acceptable for this field.
      vector<i64> candidates;

      // Column order traversal.
      for (i64 j = 0; j < num_cols; j++) {
        // this column is already ordered, so ignore this iteration.
        if (ordering[j] >= 0) {
          continue;
        }

        bool col_ok = true;
        for (i64 k = 0; k < num_rows; k++) {
          // ignore rows that are known to be wrong.
          if (wrong_ticket_idxs.count(k) != 0)
            continue;

          i64 entry = nearby_tickets[k][j];

          col_ok =
              col_ok &&
              check_rule(i, entry); // check the i'th field against this entry

          if (!col_ok)
            break;
        } // end of k

        // since num_valid_tickets is > 0, if we get to this point
        // with a col_ok of true, it must mean that all entries in
        // this j'th column agree upon the field 'i'.
        if (col_ok) {
          candidates.push_back(j);
        }
      } // end of j

      // If there is only 1 candidate column for this field, then the current
      // field must be that column's only ordering.
      if (candidates.size() == 1) {
        ordering[candidates.back()] = i;
        num_ordered_fields++;
      }
    } // end of i

    // if all possible fields have been ordered, we can stop.
    if (num_ordered_fields == ordering.size()) {
      break;
    }
  } // end of while(true)

  i64 prod = 1;
  for (i64 i = 0; i < ordering.size(); i++) {
    auto ord = ordering[i];
    if (ord >= 0 && ord <= 5) { // departure ids
      prod *= myticket[i];
    }
  }

  return prod;
}

int main() {
  fstream in("input.txt");

  i64 i = 0;
  string line;
  while (getline(in, line)) {
    if (line == "") {
      i++;
      continue;
    }

    if (i == 0) {
      parse_rule(line);
    } else if (i == 1) {
      if (line.find("your ticket") != string::npos) {
        continue;
      } else {
        vector<string> ticket = split(line, ",");
        for (auto t : ticket)
          myticket.push_back(strtoll(t.c_str(), NULL, 10));
      }
    } else if (i == 2) {
      if (line.find("nearby tickets") != string::npos) {
        continue;
      } else {
        vector<string> ticket = split(line, ",");
        vector<i64> nb_tickets;
        for (auto t : ticket)
          nb_tickets.push_back(strtoll(t.c_str(), NULL, 10));
        nearby_tickets.push_back(nb_tickets);
      }
    }
  }

  auto [wrong, sum] = solve_part_one();

  printf("Part 1 answer: %lld\n", sum);
  printf("Part 2 answer: %lld\n", solve_part_two(wrong));

  return 0;
}
