#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "sumeragi.hpp"

static char getch() {
  struct termios old_attr, new_attr;
  char c;
  tcgetattr(STDIN_FILENO, &old_attr);
  new_attr = old_attr;
  new_attr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
  c = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &old_attr);
  return c;
}

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
  on_keypressed(getch());
}

void sumeragi_t::draw() const {
  for (auto const& text : texts) {
    auto const& attr = text.attr;
    printf("\033[%d;%dH\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm%s%s\033[0m",
        text.y, text.x,
        attr.foreground.r, attr.foreground.g, attr.foreground.b,
        attr.background.r, attr.background.g, attr.background.b,
        attr.is_bold ? "\033[1m" : "",
        text.content.c_str());
  }
}

