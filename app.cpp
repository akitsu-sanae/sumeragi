#include "sumeragi.hpp"

struct app_t : public sumeragi_t {
  explicit app_t() :
    sumeragi_t{}
  {
    texts.emplace_back(0, 0, "Hello");
    texts.emplace_back(0, 1, "nekosan", attr_t{
        color_t::white(),  // foreground color
        color_t::black(),  // background color
        false               // is bold?
        });
  }

  void on_keypressed(char c) override {
    texts.emplace_back(key_count, 2, std::string{c});
    key_count++;
    if (c == 'q')
      is_quit = true;
  }
  bool is_quit = false;
  int key_count = 0;
};

int main() {
  auto app = app_t{};
  while (!app.is_quit) {
    app.update();
    app.draw();
  }
}

