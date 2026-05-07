#include <limits.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2];
  if (pipe(fd) == -1) return 1;

  pid_t pid = fork();
  if (pid == -1) return 1;

  if (pid == 0) {  // Читатель (Потомок)
    close(fd[1]);
    unsigned int val, expected = 0;
    // read возвращает 0, когда писатель закроет дескриптор
    while (read(fd[0], &val, sizeof(val)) > 0) {
      if (val != expected) {
        printf("[R] Ошибка: ждали %u, получили %u\n", expected, val);
        expected = val;
      }
      if (val % 1000000 == 0) printf("[R] Получено: %u\n", val);
      expected = (expected == UINT_MAX) ? 0 : expected + 1;
    }
    close(fd[0]);
    return 0;
  } else {  // Писатель (Родитель)
    close(fd[0]);
    unsigned int val = 0;
    // Передаем последовательность (ограничим для примера 10 млн)
    for (int i = 0; i < 10000000; i++) {
      if (write(fd[1], &val, sizeof(val)) <= 0) break;
      val = (val == UINT_MAX) ? 0 : val + 1;
    }
    close(fd[1]);  // Закрытие pipe сигнализирует читателю об окончании
    wait(NULL);    // Ждем завершения потомка
  }
  return 0;
}