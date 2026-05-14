#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

//echo "Это секретное содержимое" > secret.txt
//chmod 400 secret.txt
//gcc suid_test.c -o suid_test
//./suid_test 

//sudo -u user ./suid_test 

//chmod u+s suid_test
//sudo -u user ./suid_test 

int main() {
  char buf[256];
  int fd, n;

  printf("RUID: %d\nEUID: %d\n", getuid(), geteuid());

  if ((fd = open("secret.txt", O_RDONLY)) < 0) return perror("Error"), 1;

  while ((n = read(fd, buf, sizeof(buf))) > 0) write(1, buf, n);

  close(fd);
  return 0;
}