#ifndef SUMERAGI_HPP
#define SUMERAGI_HPP

#include <termios.h>
#include <vector>
#include <string>
#include <memory>

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

  explicit attr_t() :
    foreground{color_t::white()},
    background{color_t::black()},
    is_bold{false}
  {}
  explicit attr_t(color_t const& fore, color_t const& back, bool is_bold) :
    foreground{fore}, background{back}, is_bold{is_bold}
  {}
};

struct object_t {
  virtual void print() const = 0;
  virtual ~object_t() = default;
};

struct text_t : public object_t {
  int x, y;
  std::string content;
  attr_t attr;

  explicit text_t(int x, int y, std::string const& content, attr_t attr) :
    x{x}, y{y}, content{content}, attr{std::move(attr)}
  {}
  explicit text_t(int x, int y, std::string const& content) :
    x{x}, y{y}, content{content}
  {}

  void print() const override;
};

struct powerline_t : public object_t {
  struct elem_t {
    std::string content;
    attr_t attr;

    explicit elem_t(std::string const& content, attr_t const& attr) :
      content{content}, attr{attr}
    {}
  };
  int x, y;
  std::vector<elem_t> line;

  explicit powerline_t(int x, int y, std::vector<elem_t> const& elems) :
    x{x}, y{y}, line{elems}
  {}
  explicit powerline_t(int x, int y) :
    x{x}, y{y}
  {}
  void print() const override;
};

struct sumeragi_t {
  explicit sumeragi_t();
  void update();
  void draw() const;
  virtual void on_keypressed(char c) = 0;
  ~sumeragi_t();
protected:
  std::vector<std::shared_ptr<object_t>> objects;
private:
  struct termios origin;
  int inout;
};

#endif

