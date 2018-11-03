#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "sumeragi.hpp"

sumeragi_t::sumeragi_t() {
  int inout = open("/dev/tty", O_RDWR);

  tcgetattr(inout, &origin);
  struct termios term;
  memcpy(&term, &origin, sizeof(struct termios));

  // raw mode
  term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
      | INLCR | IGNCR | ICRNL | IXON);
  term.c_oflag &= ~OPOST;
  term.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  term.c_cflag &= ~(CSIZE | PARENB);
  term.c_cflag |= CS8;
  term.c_cc[VMIN] = 0;
  term.c_cc[VTIME] = 1;
  tcsetattr(inout, TCSAFLUSH, &term);

  printf("\033[2J\033[1;1H");
}

sumeragi_t::~sumeragi_t() {
  tcsetattr(inout, TCSAFLUSH, &origin);
}

void sumeragi_t::update() {
  on_keypressed(getchar());
}

void text_t::print() const {
  printf("\033[%d;%dH\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm%s%s\033[0m",
      y+1, x+1,
      attr.foreground.r, attr.foreground.g, attr.foreground.b,
      attr.background.r, attr.background.g, attr.background.b,
      attr.is_bold ? "\033[1m" : "",
      content.c_str());
}

void powerline_t::print() const {
  printf("\033[%d;%dH", y+1, x+1);
  for (int i=0; i<line.size(); ++i) {
    auto const& attr = line[i].attr;
    auto const& content = line[i].content;
    printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm%s %s ",
      attr.foreground.r, attr.foreground.g, attr.foreground.b,
      attr.background.r, attr.background.g, attr.background.b,
      attr.is_bold ? "\033[1m" : "",
      content.c_str());
    if (i+1 != line.size()) {
      auto const& fore = line[i].attr.background;
      auto const& back = line[i+1].attr.background;
      printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm\xEE\x82\xB0",
        fore.r, fore.g, fore.b,
        back.r, back.g, back.b);
    }
  }
  printf("\033[0m");
}

void sumeragi_t::draw() const {
  for (auto const& obj : objects)
    obj->print();
}

