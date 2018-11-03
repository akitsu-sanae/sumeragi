#include "sumeragi.hpp"
#include <stdio.h>
#include <cmath>


struct app_t : public sumeragi_t {
  explicit app_t() :
    sumeragi_t{}
  {
    objects.push_back(std::make_shared<text_t>(0, 0, "Hello"));
    std::vector<powerline_t::elem_t> line;
    line.emplace_back(
        "hoge",
        attr_t {
            color_t::white(),
            color_t::red(),
            false
        });
    line.emplace_back(
        "fuga",
        attr_t {
            color_t::white(),
            color_t::green(),
            false
        });
    line.emplace_back(
        "piyo",
        attr_t {
            color_t::white(),
            color_t::blue(),
            false
        });
    line.emplace_back(
        "nyan",
        attr_t {
            color_t::black(),
            color_t::white(),
            false
        });

    objects.push_back(std::make_shared<powerline_t>(0, 1, line));

    objects.push_back(std::make_shared<text_t>(0, 2, "nekosan", attr_t{
        color_t::magenta(),  // foreground color
        color_t::black(),  // background color
        true               // is bold?
        }));
  }

  void on_keypressed(char c) override {
    int H = count;
    double f = H/60.0 - std::floor(H/60.0);
    int V = 255;
    int M = 0;
    int N = 255 * (1-f);
    int K = 255 * f;
    color_t color{};
    switch ((int)(H/60.0)) {
    case 0:
      color.r = 255; color.g = K; color.b = M;
      break;
    case 1:
      color.r = N; color.g = V; color.b = M;
      break;
    case 2:
      color.r = M; color.g = V; color.b = K;
      break;
    case 3:
      color.r = M; color.g = N; color.b = V;
      break;
    case 4:
      color.r = K; color.g = M; color.b = V;
      break;
    case 5:
      color.r = V; color.g = M; color.b = N;
      break;
    }
    objects.push_back(std::make_shared<text_t>(count/10, line_count, std::string{c}, attr_t {
        color, color_t::black(), false
    }));
    count = (count + 10) % 360;
    if (count == 0)
      ++line_count;
    if (c == 'q')
      is_quit = true;
  }
  bool is_quit = false;
  int count = 0;
  int line_count = 3;
};

int main() {
  auto app = app_t{};
  while (!app.is_quit) {
    app.draw();
    app.update();
  }
}

