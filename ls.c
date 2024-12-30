#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  int list_all;
} ls_option_t;

int apply_option(char new_option, ls_option_t *opt_t) {
  switch (new_option) {
  case 'a':
    opt_t->list_all = 1;
    break;
  default:
    break;
  }
  return 0;
}

int list_dir(const char *dirname, int out_fd) {
  DIR *dir = opendir(dirname);
  if (dir == NULL) {
    perror("Dir open failed");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    write(out_fd, entry->d_name, strlen(entry->d_name));
    char *endl = "\n";
    write(out_fd, endl, sizeof(char));
  }
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {

  if (argc < 2) {
    int status_code = list_dir(".", STDOUT_FILENO);
    return status_code;
  }

  printf("Hello world");
  return EXIT_SUCCESS;
}
