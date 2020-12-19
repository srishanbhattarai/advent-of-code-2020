#include <fstream>
#include <vector>

using namespace std;

using u64 = unsigned long long;

enum TokenType { Add, Mul, Lparen, Rparen, Num };

struct Token {
  TokenType type;

  // position of the token
  int line_number;
  int column_number;

  char token;
};

vector<Token> tokenize(string line, int line_number) {
  vector<Token> tokens;

  for (int col = 0; col < line.size(); col++) {
    char token = line[col];

    if (token == ' ')
      continue;

    Token t;
    t.line_number = line_number;
    t.column_number = col;
    t.token = token;

    if (token == '+')
      t.type = TokenType::Add;
    else if (token == '*')
      t.type = TokenType::Mul;
    else if (token == '(')
      t.type = TokenType::Lparen;
    else if (token == ')')
      t.type = TokenType::Rparen;
    else
      t.type = TokenType::Num;

    tokens.push_back(t);
  }

  return tokens;
}

// pair.first = offset
// pair.second = expr result
pair<u64, u64> parse_expr(vector<Token> &tokens, u64 offset = 0) {
  u64 i = offset; // new offset

  Token t = tokens[i];

  u64 lhs = 0;

  // Evaluate LHS.
  if (t.type == TokenType::Num) {
    lhs = atoi(&t.token);
    i += 1;
  } else if (t.type == TokenType::Lparen) {
    auto result = parse_expr(tokens, i + 1);
    i = result.first;
    lhs = result.second;
  } else {
    assert(false);
  }

  // End of line, end recursion.
  if (i == tokens.size())
    return {i, lhs};

  // start evaluating RHS.
  Token next = tokens[i];
  if (next.type == TokenType::Add) {
    auto result = parse_expr(tokens, i + 1);
    return {result.first, result.second + lhs};
  } else if (next.type == TokenType::Mul) {
    auto result = parse_expr(tokens, i + 1);
    return {result.first, result.second * lhs};
  } else if (next.type == TokenType::Rparen) {
    return {i + 1, lhs};
  } else {
    assert(false);
  }
}

int main() {
  fstream in("input.txt");

  string line;
  int i = 0;
  u64 sum = 0;
  while (getline(in, line)) {
    vector<Token> tokens = tokenize(line, i + 1);

    // parse_expr is right associative so just reverse the vector brrrr...
    // (and flip the parens)
    reverse(tokens.begin(), tokens.end());
    for (auto &t : tokens) {
      if (t.type == TokenType::Lparen)
        t.type = TokenType::Rparen;
      else if (t.type == TokenType::Rparen)
        t.type = TokenType::Lparen;
    }

    auto [_, expr_result] = parse_expr(tokens);
    sum += expr_result;
  }

  printf("Answer: %lld\n", sum);

  return 0;
}
