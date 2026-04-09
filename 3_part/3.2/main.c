#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void create_dir(char *path) { mkdir(path, 0755); }

void list_dir(char *path) {
  DIR *d = opendir(path);
  struct dirent *e;
  while ((e = readdir(d))) printf("%s\n", e->d_name);
  closedir(d);  
}

void remove_dir(char *path) { rmdir(path); }

void create_file(char *path) {
  int fd = open(path, O_CREAT | O_WRONLY);
  close(fd);
}

void print_file(char *path) {
  char buf[256];
  int fd = open(path, O_RDONLY);
  int n;
  while ((n = read(fd, buf, sizeof(buf))) > 0) write(1, buf, n);
  close(fd);
}

void remove_file(char *path) { unlink(path); }

void create_symlink(char *target, char *linkname) { symlink(target, linkname); }

void read_symlink(char *path) {
  char buf[256];
  int n = readlink(path, buf, sizeof(buf) - 1);
  buf[n] = 0;
  printf("%s\n", buf);
}

void print_symlink_file(char *path) {
  char buf[256];
  int n = readlink(path, buf, sizeof(buf) - 1);
  buf[n] = 0;
  print_file(buf);
}

void create_hardlink(char *target, char *linkname) { link(target, linkname); }
//ln q.txt w.txt


void file_info(char *path) {
  struct stat st;
  stat(path, &st);
  printf("Permissions: %o\n", st.st_mode & 0777);
  printf("Hard links: %ld\n", st.st_nlink);
}

void change_perm(char *path, char *mode) { chmod(path, strtol(mode, NULL, 8)); }

int main(int argc, char *argv[]) {
  char *cmd = strrchr(argv[0], '/');
  cmd = cmd ? cmd + 1 : argv[0];

  if (!strcmp(cmd, "mkdir_cmd"))
    create_dir(argv[1]);
  else if (!strcmp(cmd, "ls_cmd"))
    list_dir(argv[1]);
  else if (!strcmp(cmd, "rmdir_cmd"))
    remove_dir(argv[1]);

  else if (!strcmp(cmd, "touch_cmd"))
    create_file(argv[1]);
  else if (!strcmp(cmd, "cat_cmd"))
    print_file(argv[1]);
  else if (!strcmp(cmd, "rm_cmd"))
    remove_file(argv[1]);

  else if (!strcmp(cmd, "symlink_cmd"))
    create_symlink(argv[1], argv[2]);
  else if (!strcmp(cmd, "readlink_cmd"))
    read_symlink(argv[1]);
  else if (!strcmp(cmd, "catlink_cmd"))
    print_symlink_file(argv[1]);
  else if (!strcmp(cmd, "rmlink_cmd"))
    remove_file(argv[1]);

  else if (!strcmp(cmd, "hardlink_cmd"))
    create_hardlink(argv[1], argv[2]);
  else if (!strcmp(cmd, "rmhard_cmd"))
    remove_file(argv[1]);

  else if (!strcmp(cmd, "stat_cmd"))
    file_info(argv[1]);
  else if (!strcmp(cmd, "chmod_cmd"))
    change_perm(argv[1], argv[2]);

  else
    printf("Unknown command\n");

  return 0;
}
