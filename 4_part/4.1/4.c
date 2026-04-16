#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int global_init = 10;          // 4.1.4 Инициализированная
int global_uninit;             // 4.1.5 Неинициализированная
const int global_const = 100;  // 4.1.6 Глобальная константа
extern char **environ;

void demo_addresses() {
  int local_var = 1;          // 4.1.1 Локальная
  static int static_var = 2;  // 4.1.2 Статическая
  const int local_const = 3;  // 4.1.3 Локальная константа

  printf("--- Адреса переменных ---\n");
  printf("Локальная:             %p\n", (void *)&local_var);
  printf("Локальная константа:    %p\n", (void *)&local_const);
  printf("Статическая:           %p\n", (void *)&static_var);
  printf("Глобальная init:       %p\n", (void *)&global_init);
  printf("Глобальная uninit:     %p\n", (void *)&global_uninit);
  printf("Глобальная константа:  %p\n", (void *)&global_const);
}

// 1.4 адрес локальной переменной
long long get_local_addr() {
  int temp = 42;
  return (long long)&temp;
}

void heap_demo() {
  printf("\n--- Heap ---\n");
  char *ptr = malloc(100);
  strcpy(ptr, "Hello World");
  printf("Буфер: %s\n", ptr);

  free(ptr);
  printf("После free: %s (UB!)\n", ptr);

  char *ptr2 = malloc(100);
  strcpy(ptr2, "New Data");
  printf("Новый буфер: %s\n", ptr2);

  char *mid_ptr = ptr2 + 3;
  printf("После free: %s (UB!)\n", mid_ptr);
  printf("free из середины - crash.\n");
}

int main() {
  demo_addresses();
  getchar();
  // 1.4
  long long bad_ptr = get_local_addr();
  printf("\nАдрес из функции: %p, Значение: %d \n", (void *)bad_ptr, bad_ptr);

  heap_demo();

  // 1.8 Переменные окружения
  char *env_val = getenv("MY_VAR");
  printf("\nMY_VAR: %s\n", env_val ? env_val : "не найдена");
  setenv("MY_VAR", "NEW_VALUE", 1);
  printf("MY_VAR (измененная): %s\n", getenv("MY_VAR"));

  printf("\nPID процесса: %d. Нажмите Enter, чтобы завершить...\n", getpid());
  getchar();

  for (char **env = environ; *env != NULL; env++) {
    printf("%s\n", *env);
  }
  return 0;
}