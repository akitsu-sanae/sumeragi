#ifndef SUMERAGI_HPP
#define SUMERAGI_HPP

#include <termios.h>
#include <vector>
#include <string>

struct color_t {
  unsigned char red, green, blue;
  explicit color_t(unsigned char r, unsigned char g, unsigned char b) :
    red{r}, green{g}, blue{b}
  {}
  explicit color_t() :
    red{}, green{}, blue{}
  {}
  static color_t white() {
    return color_t{255, 255, 255};
  }
  static color_t black() {
    return color_t{0, 0, 0};
  }
};

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

