#include <string>
#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <locale>
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
constexpr double scale = 1000.0;

vector<string> split(const string& s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}

static inline void ltrim(std::string& s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

static inline void rtrim(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

static inline void trim(std::string& s) {
  ltrim(s);
  rtrim(s);
}

int main(int argc, char** argv) try {
  ifstream infile(argv[1]);

  int64_t asm_gap = 0, copper_gap = 0, silk_len = 0;

  string line;
  while (std::getline(infile, line)) {
    // remove trailing newline
    trim(line);

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
      fmt::print("Assembly begins\n");
    } else if (input_tag == "copper") {
      fmt::print("Copper begins\n");
    } else if (input_tag == "line") {
    } else if (input_tag == "arc") {
    } else {
      fmt::print("Unknown tag named '{}'\n", input_tag);
    }
  }

  return 0;
} catch (const std::exception& e) {
  fmt::print("Something bad happened: {}\n", e.what());
  std::exit(1);
}
