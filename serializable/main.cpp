#include <iostream>
#include <string>

#include "operation.hpp"
#include "parser.hpp"
#include "serializable.hpp"


int main() {

// Read the input string
  std::string input_buffer;
  std::getline(std::cin, input_buffer);
  input_buffer.erase(input_buffer.find_last_not_of(" \n\r\t") + 1);

  // Parse the input and construct a history
  parser::input_parser parser{input_buffer};
  //std::cout << input_buffer << std::endl;
  operations_t history = parser.parse();

  // Output the result
  std::cout << (is_serializable(history) ? "true" : "false") << std::endl;
  return 0;
}
