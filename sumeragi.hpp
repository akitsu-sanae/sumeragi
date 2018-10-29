#ifndef SUMERAGI_HPP
#define SUMERAGI_HPP

#include <termios.h>
#include <vector>
#include <string>

#define DEF_COLOR(X, R, G, B) static color_t X() {\
  return color_t{R, G, B}; \
}

struct color_t {
  unsigned char r, g, b;
  explicit color_t(unsigned char r, unsigned char g, unsigned char b) :
    r{r}, g{g}, b{b}
  {}
  explicit color_t() :
    r{}, g{}, b{}
  {}
  DEF_COLOR(white, 255, 255, 255)
  DEF_COLOR(black,   0,   0,   0)
  DEF_COLOR(red,   255,   0,   0)
  DEF_COLOR(green,   0, 255,   0)
  DEF_COLOR(blue,    0,   0, 255)
  DEF_COLOR(cyan,    0, 255, 255)
  DEF_COLOR(magenta, 255, 0, 255)
  DEF_COLOR(yellow,  0, 255, 255)
};

#undef DEF_COLOR

struct attr_t {
  color_t foreground;
  color_t background;
  bool is_bold;
};

struct text_t {
  int x, y;
  std::string content;
  attr_t attr;

  explicit text_t(int x, int y, std::string const& content, attr_t attr) :
    x{x}, y{y}, content{content}, attr{std::move(attr)}
  {}
  explicit text_t(int x, int y, std::string const& content) :
    x{x}, y{y}, content{content}
  {}
};

struct sumeragi_t {
  explicit sumeragi_t();
  void update();
  void draw() const;
  virtual void on_keypressed(char c) = 0;
  ~sumeragi_t();
protected:
  std::vector<text_t> texts;
private:
  struct termios origin;
  int inout;
};

#endif

