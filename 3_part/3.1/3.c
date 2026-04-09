#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void rev(char *s) {
  int i = 0, j = strlen(s) - 1;
  while (i < j) {
    char c = s[i];
    s[i++] = s[j];
    s[j--] = c;
  }
}

int main(int c, char **v) {
  DIR *d = opendir(v[1]);
  struct dirent *e;
  struct stat st;

  char path[512], newpath[512];
  char dir[256], name[256], ext[256];

  strcpy(dir, strrchr(v[1], '/') ? strrchr(v[1], '/') + 1 : v[1]);
  rev(dir);
  mkdir(dir, 0777);

  while ((e = readdir(d))) {
    sprintf(path, "%s/%s", v[1], e->d_name);

    if (!stat(path, &st) && (st.st_mode & S_IFREG)) {
      char *dot = strrchr(e->d_name, '.');

      if (dot) {
        strncpy(name, e->d_name, dot - e->d_name);
        name[dot - e->d_name] = 0;
        strcpy(ext, dot);
      } else {
        strcpy(name, e->d_name);
        ext[0] = 0;
      }

      rev(name);

      sprintf(newpath, "%s/%s%s", dir, name, ext);

      FILE *f = fopen(path, "rb");
      FILE *g = fopen(newpath, "wb");

      fseek(f, 0, SEEK_END);
      long i = ftell(f);

      while (i--) {
        fseek(f, i, SEEK_SET);
        fputc(fgetc(f), g);
      }

      fclose(f);
      fclose(g);
    }
  }
}
