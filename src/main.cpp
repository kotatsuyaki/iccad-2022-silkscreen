#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/ostream.h>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::ifstream;
using std::ofstream;
using std::string;
using std::stringstream;
using std::vector;

const char* const INPUT_FILENAME = "./Problem.txt";
const char* const OUTPUT_FILENAME = "./Result.txt";
constexpr double scale = 10000.0;
constexpr int64_t extra_gap = 100;

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

struct Point {
  Point(int64_t x, int64_t y) : x(x), y(y) {}
  int64_t x, y;
};

struct SilkBox {
  SilkBox() {}

  void update_box(Point pt1, Point pt2) {
    min_x = std::min(pt1.x, min_x);
    min_x = std::min(pt2.x, min_x);
    max_x = std::max(pt1.x, max_x);
    max_x = std::max(pt2.x, max_x);
    min_y = std::min(pt1.y, min_y);
    min_y = std::min(pt2.y, min_y);
    max_y = std::max(pt1.y, max_y);
    max_y = std::max(pt2.y, max_y);
  }

  void update_arc(Point start, Point center) {
    // find bounding box
    const double x1 = static_cast<double>(start.x);
    const double y1 = static_cast<double>(start.y);
    const double x2 = static_cast<double>(center.x);
    const double y2 = static_cast<double>(center.y);

    const double x_len = x1 - x2;
    const double y_len = y1 - y2;
    const double radius = std::sqrt((x_len * x_len) + (y_len * y_len));

    const int64_t radius_i = static_cast<int64_t>(std::ceil(radius));

    fmt::print("Arc has radius = {}\n", radius_i);

    const Point pt1{
        center.x + radius_i,
        center.y + radius_i,
    };
    const Point pt2{
        center.x - radius_i,
        center.y - radius_i,
    };

    // delegate to update_box
    update_box(pt1, pt2);
  }

  std::pair<Point, Point> enlarged(int64_t gap) const {
    const Point pt1{
        min_x - gap,
        min_y - gap,
    };
    const Point pt2{
        max_x + gap,
        max_y + gap,
    };
    return {pt1, pt2};
  }

  int64_t min_x = std::numeric_limits<int64_t>::max();
  int64_t max_x = std::numeric_limits<int64_t>::min();
  int64_t min_y = std::numeric_limits<int64_t>::max();
  int64_t max_y = std::numeric_limits<int64_t>::min();
};

int64_t scaled_int(const string& s) {
  return static_cast<int64_t>(std::stod(s) * scale);
}

double scaled_dbl(int64_t n) { return static_cast<double>(n) / scale; }

int main() try {
  ifstream infile(INPUT_FILENAME);

  int64_t asm_gap = 0, copper_gap = 0, silk_len = 0;

  SilkBox silk_box{};

  string line;
  while (std::getline(infile, line)) {
    // remove trailing newline
    trim(line);

    const auto tokens = split(line, ',');
    const auto input_tag = tokens.at(0);

    if (input_tag == "assemblygap") {
      asm_gap = scaled_int(tokens.at(1));
    } else if (input_tag == "coppergap") {
      copper_gap = scaled_int(tokens.at(1));
    } else if (input_tag == "silkscreenlen") {
      silk_len = scaled_int(tokens.at(1));

      fmt::print("Params = {}, {}, {}\n", asm_gap, copper_gap, silk_len);
    } else if (input_tag == "assembly") {
      fmt::print("Assembly begins\n");
    } else if (input_tag == "copper") {
      fmt::print("Copper begins\n");
    } else if (input_tag == "line") {
      fmt::print("Line\n");

      const Point pt1{
          scaled_int(tokens.at(1)),
          scaled_int(tokens.at(2)),
      };
      const Point pt2{
          scaled_int(tokens.at(3)),
          scaled_int(tokens.at(4)),
      };

      silk_box.update_box(pt1, pt2);
    } else if (input_tag == "arc") {
      fmt::print("Arc\n");

      const Point start{
          scaled_int(tokens.at(1)),
          scaled_int(tokens.at(2)),
      };
      const Point center{
          scaled_int(tokens.at(5)),
          scaled_int(tokens.at(6)),
      };

      silk_box.update_arc(start, center);
    } else {
      fmt::print("Unknown tag named '{}'\n", input_tag);
    }
  }

  fmt::print("Bounding box: ({}, {}) to ({}, {})\n",  //
             silk_box.min_x, silk_box.min_y,          //
             silk_box.max_x, silk_box.max_y);

  const auto [pt1, pt2] =
      silk_box.enlarged(std::max(copper_gap, asm_gap) + extra_gap);

  fmt::print("Enlarged box: ({}, {}) to ({}, {})\n",  //
             pt1.x, pt1.y,                            //
             pt2.x, pt2.y);

  ofstream outfile(OUTPUT_FILENAME);

  // CCW walk
  fmt::print(outfile, "silkscreen\n");
  fmt::print(outfile, "line,{:.4f},{:.4f},{:.4f},{:.4f}\n",  //
             scaled_dbl(pt1.x), scaled_dbl(pt1.y),           //
             scaled_dbl(pt2.x), scaled_dbl(pt1.y));
  fmt::print(outfile, "line,{:.4f},{:.4f},{:.4f},{:.4f}\n",  //
             scaled_dbl(pt2.x), scaled_dbl(pt1.y),           //
             scaled_dbl(pt2.x), scaled_dbl(pt2.y));
  fmt::print(outfile, "line,{:.4f},{:.4f},{:.4f},{:.4f}\n",  //
             scaled_dbl(pt2.x), scaled_dbl(pt2.y),           //
             scaled_dbl(pt1.x), scaled_dbl(pt2.y));
  fmt::print(outfile, "line,{:.4f},{:.4f},{:.4f},{:.4f}\n",  //
             scaled_dbl(pt1.x), scaled_dbl(pt2.y),           //
             scaled_dbl(pt1.x), scaled_dbl(pt1.y));

  return 0;
} catch (const std::exception& e) {
  fmt::print("Something bad happened: {}\n", e.what());
  std::exit(1);
}
