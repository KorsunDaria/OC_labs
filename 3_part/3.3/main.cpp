#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_pagemap(int pid, uintptr_t addr) {
  char path[64];
  uint64_t entry;
  uint64_t page_size = sysconf(_SC_PAGESIZE);

  // Формируем путь и открываем файл
  snprintf(path, sizeof(path), "/proc/%d/pagemap", pid);
  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    perror("Ошибка открытия (нужен root?)");
    return;
  }

  // Смещение = (вирт_адрес / размер_страницы) * 8 байт
  if (lseek(fd, (addr / page_size) * sizeof(uint64_t), SEEK_SET) == -1) {
    perror("Ошибка lseek");
    close(fd);
    return;
  }

  if (read(fd, &entry, sizeof(uint64_t)) != sizeof(uint64_t)) {
    perror("Ошибка чтения");
  } else {
    int present = (entry >> 63) & 1;
    int swapped = (entry >> 62) & 1;
    uint64_t pfn = entry & 0x7FFFFFFFFFFFFF;

    printf("Адрес: 0x%lx\n", addr);
    printf("Present: %s | Swapped: %s\n", present ? "Да" : "Нет",
           swapped ? "Да" : "Нет");
    if (present) {
      printf("PFN: 0x%lx | Физический адрес: 0x%lx\n", pfn, pfn * page_size);
    }
  }
  close(fd);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Использование: sudo %s <PID> [ADDR]\n", argv[0]);
    return 1;
  }

  int pid = atoi(argv[1]);
  // Если адрес не указан, берем адрес локальной переменной (стек)
  uintptr_t addr = (argc > 2) ? strtoul(argv[2], NULL, 16) : (uintptr_t)&pid;

  print_pagemap(pid, addr);
  return 0;
}