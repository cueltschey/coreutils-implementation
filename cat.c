#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <unistd.h>

#define BUF_SIZE 1024

typedef struct {
  int number;
  int number_nonblank;
  int squeeze_blank;
  int show_ends;
  int show_nonprinting;
  int show_tabs;
} cat_options_t;

const char *usage() { return "Usage: cat <options> [file(s)]\n"; }

void apply_option(char c, cat_options_t *opt_t) {
  switch (c) {
  case 'b':
    opt_t->number = 1;
    opt_t->number_nonblank = 1;
    break;

  case 'e':
    opt_t->show_ends = 1;
    opt_t->show_nonprinting = 1;
    break;

  case 'n':
    opt_t->number = 1;
    break;

  case 's':
    opt_t->squeeze_blank = 1;
    break;

  case 't':
    opt_t->show_tabs = 1;
    opt_t->show_nonprinting = 1;
    break;

  case 'u':
    /* We provide the -u feature unconditionally.  */
    break;

  case 'v':
    opt_t->show_nonprinting = 1;
    break;

  case 'A':
    opt_t->show_nonprinting = 1;
    opt_t->show_ends = 1;
    opt_t->show_tabs = 1;
    break;

  case 'E':
    opt_t->show_ends = 1;
    break;

  case 'T':
    opt_t->show_tabs = 1;
    break;
  default:
    usage();
    exit(EXIT_FAILURE);
  }
}

int file_to_stdout(const char *filename, const cat_options_t options) {
  int input_fd = open(filename, O_RDONLY);
  if (input_fd < 0) {
    perror("file open failed");
    return -1;
  }
  char buf;
  char prev = '\n';
  int bytes_read = 0;
  int line_number = 0;
  while ((bytes_read = read(input_fd, &buf, sizeof(char))) > 0) {
    if (buf == '\n' && options.show_ends) {
      char end = '$';
      write(STDOUT_FILENO, &end, sizeof(char));
    }
    write(STDOUT_FILENO, &buf, sizeof(char));
    if (buf == '\n' && (options.number || options.number_nonblank)) {
      if (options.number_nonblank) {
        if (prev == '\n')
          continue;
        line_number++;
        char *number_string = (char *)malloc(32 * sizeof(char));
        int tmp = line_number;
        while (tmp > 0) {
          *number_string = (tmp % 10) + '0';
          number_string++;
          tmp -= (tmp % 10);
          tmp /= 10;
        }
        write(STDOUT_FILENO, number_string, strlen(number_string));
      } else {
        line_number++;
        char *number_string = "";
        int tmp = line_number;
        while (tmp > 0) {
          *number_string = (tmp % 10) + '0';
          tmp -= (tmp % 10);
          tmp /= 10;
        }
        write(STDOUT_FILENO, number_string, strlen(number_string));
      }
    }
    prev = buf;
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("%s", usage());
  }

  cat_options_t options = {0, 0, 0, 0, 0, 0};

  int file_index = 0;

  while (++file_index < argc) {
    if (strlen(argv[file_index]) > 1 && argv[file_index][0] == '-') {
      apply_option(argv[file_index][1], &options);
      continue;
    }
    file_to_stdout(argv[file_index], options);
  }

  return EXIT_SUCCESS;
}
