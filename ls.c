#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

int list_dir(const char *dirname, int out_fd, ls_option_t options) {
  struct stat file_stat;
  if (stat(dirname, &file_stat) != 0) {
    perror("Stat failed");
    return EXIT_FAILURE;
  }
  if (S_ISDIR(file_stat.st_mode) == 0) {
    write(out_fd, dirname, strlen(dirname));
    char *endl = "\n";
    write(out_fd, endl, sizeof(char));
    return EXIT_SUCCESS;
  }

  DIR *dir = opendir(dirname);
  if (dir == NULL) {
    perror("Dir open failed");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (!options.list_all && entry->d_name[0] == '.')
      continue;
    write(out_fd, entry->d_name, strlen(entry->d_name));
    char *endl = "\n";
    write(out_fd, endl, sizeof(char));
  }
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {

  ls_option_t options = {0};

  int file_provided = 0;
  int status_code;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
      apply_option(argv[i][1], &options);
      continue;
    }
    file_provided = 1;
    status_code = list_dir(argv[i], STDERR_FILENO, options);
    if (status_code == EXIT_FAILURE) {
      printf("Failed to open file %s\n", argv[1]);
    }
  }

  if (file_provided == 0)
    status_code = list_dir(".", STDOUT_FILENO, options);
  return status_code;
}
