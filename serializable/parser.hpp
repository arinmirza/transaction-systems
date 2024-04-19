#pragma once

#include "operation.hpp"
#include <string>

namespace parser {

class input_parser {
public:
  explicit input_parser(std::string input) : input(input){};
  operations_t parse();

private:
  void skip_whitespace();
  std::string read_keyword();
  int read_number();
  bool done();

  std::string input;
  uint pos{0};
};

} // namespace parser