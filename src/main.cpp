#include <string>
#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include <cassert>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

using std::cerr;
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

const char* const INPUT_FILENAME = "Problem.txt";
const char* const OUTPUT_FILENAME = "Result.txt";
constexpr double scale = 10000.0;

vector<string> split(const string& s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}

int main() try {
  ifstream infile(INPUT_FILENAME);

  int64_t asm_gap = 0, copper_gap = 0, silk_len = 0;

  string line;
  while (std::getline(infile, line)) {
    const auto tokens = split(line, ',');
    const auto input_tag = tokens.at(0);

    if (input_tag == "assemblygap") {
      asm_gap = static_cast<int64_t>(std::stod(tokens.at(1)) * scale);
    } else if (input_tag == "coppergap") {
      copper_gap = static_cast<int64_t>(std::stod(tokens.at(1)) * scale);
    } else if (input_tag == "silkscreenlen") {
      silk_len = static_cast<int64_t>(std::stod(tokens.at(1)) * scale);
      fmt::print("Params = {}, {}, {}\n", asm_gap, copper_gap, silk_len);
    } else if (input_tag == "assembly") {
    } else if (input_tag == "copper") {
    } else {
    }
  }

  return 0;
} catch (const std::exception& e) {
  fmt::print("Something bad happened: {}\n", e.what());
  std::exit(1);
}
