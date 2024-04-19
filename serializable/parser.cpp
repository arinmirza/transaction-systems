#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>

#include "parser.hpp"
#include "item.hpp"
#include "transaction.hpp"
#include "operation.hpp"

namespace parser {


  operations_t input_parser::parse() {

    operations_t operations{};

    while (!this->done()) {
      const std::string command = read_keyword();

      if (command == "w") {
        const t_id transaction = read_number();
        const item_id item = read_keyword();
        operations.emplace_back(write_op{transaction, item});
        continue;
      }

      else if (command == "r") {
        const t_id transaction = read_number();
        const item_id item = read_keyword();
        operations.emplace_back(read_op{transaction, item});
        continue;
      }

      else if (command == "a") {
        const t_id transaction = read_number();
        operations.emplace_back(abort_op{transaction});
        continue;
      }

      else if (command == "c") {
        const t_id transaction = read_number();
        operations.emplace_back(commit_op{transaction});
        continue;
      }

      else {
        std::string msg{"Unknown command: "};
        msg += command;
        throw std::invalid_argument(msg);
      }
    }

    return operations;
  }

  void input_parser::skip_whitespace() {
    while (std::cbegin(input) + pos != std::cend(input) &&
           *(std::cbegin(input) + pos) == ' ') {
      pos++;
    }
  }

  std::string input_parser::read_keyword() {
    std::string keyword{};
    std::cout << "Reading keyword from [" << pos << "]"; 

    skip_whitespace();
    while (std::cbegin(input) + pos != std::cend(input)) {
      if (*(std::cbegin(input) + pos) == ' ') {
        break;
      } else {
        keyword += input[pos++];
      }
    }

    std::cout << " and finishing at [" << pos << "] which is " << keyword << std::endl;
    return keyword;
  }

  int input_parser::read_number() {
    std::string number_string;

    skip_whitespace();
    while (std::cbegin(input) + pos != std::cend(input)) {
      if (*(std::cbegin(input) + pos) == ' ') {
        break;
      } else {
        number_string += input[pos++];
      }
    }
    return std::stoi(number_string);
  }

  bool input_parser::done() { return std::cbegin(input) + pos == std::cend(input); }

  std::string input;
  uint pos{0};


}