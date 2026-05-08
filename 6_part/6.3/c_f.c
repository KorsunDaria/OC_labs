#include <stdio.h>   // printf, fgets, perror — стандартный ввод/вывод
#include <stdlib.h>  // exit, EXIT_FAILURE — управление процессом
#include <string.h>  // memset, strncpy, strlen — работа со строками и памятью
#include <sys/socket.h>  // socket, connect — API сокетов
#include <sys/un.h>      // struct sockaddr_un — структура адреса Unix-сокета
#include <unistd.h>      // read, write, close — низкоуровневые POSIX-вызовы

// Путь к файлу сокета — должен совпадать с тем, что задан на сервере
#define SOCKET_PATH "/tmp/echo_sock"

// Размер буфера: сколько байт максимум читаем за один раз
#define BUFFER_SIZE 256

int main() {
  int fd;
  // fd — единственный дескриптор клиента: через него и отправляем, и получаем.
  // В отличие от сервера, здесь нет разделения на server_fd и client_fd

  struct sockaddr_un addr;
  // Структура адреса Unix-сокета:
  //   .sun_family — тип домена (AF_UNIX)
  //   .sun_path   — путь к файлу сокета в файловой системе

  char buffer[BUFFER_SIZE];
  // Один буфер используется дважды:
  //   1) fgets пишет сюда ввод пользователя
  //   2) read перезаписывает сюда же ответ сервера

  // ── 1. СОЗДАНИЕ СОКЕТА ───────────────────────────────────────────────
  // socket(domain, type, protocol)
  //   AF_UNIX     — локальный сокет (файл, не сеть)
  //   SOCK_STREAM — поточный, надёжный (аналог TCP)
  //   0           — протокол выбирается автоматически по domain+type
  // Возвращает дескриптор или -1 при ошибке
  if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");  // выводит системное сообщение об ошибке в stderr
    exit(EXIT_FAILURE);      // завершает процесс с кодом ошибки (1)
  }

  // ── ПОДГОТОВКА АДРЕСА ────────────────────────────────────────────────
  // memset(ptr, value, size) — обнуляет всю структуру addr
  // Без этого в sun_path могут оказаться мусорные байты от предыдущих данных
  // стека
  memset(&addr, 0, sizeof(addr));

  addr.sun_family = AF_UNIX;
  // Указываем домен — Unix-сокет (локальный, через файловую систему)

  // strncpy(dst, src, n) — копирует путь в поле sun_path
  // sizeof(addr.sun_path) - 1 — оставляем место для нуль-терминатора \0
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  // ── 2. ПОДКЛЮЧЕНИЕ К СЕРВЕРУ ─────────────────────────────────────────
  // connect(fd, addr, addrlen) — устанавливает соединение с сервером
  //   fd              — наш сокет
  //   (struct sockaddr*) — приведение типа: connect принимает обобщённый адрес
  //   sizeof(addr)    — размер конкретной структуры sockaddr_un
  // Блокирует до завершения рукопожатия или ошибки.
  // В отличие от сервера здесь нет bind() — адрес клиенту не нужен
  if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(EXIT_FAILURE);
  }

  printf("Введите сообщение (для выхода Ctrl+C):\n");

  // ── 3. ГЛАВНЫЙ ЦИКЛ ОБМЕНА ───────────────────────────────────────────
  // fgets(buf, size, stream) — читает строку из потока stdin
  //   buffer        — куда записать
  //   sizeof(buffer) — максимум символов (включая \n и \0)
  //   stdin         — стандартный ввод (клавиатура)
  // Возвращает NULL при EOF (Ctrl+D) или ошибке → выходим из цикла
  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    // write(fd, buf, count) — отправляет данные серверу
    //   fd            — дескриптор соединения
    //   buffer        — указатель на данные
    //   strlen(buffer) — длина строки в байтах (без \0, но с \n)
    // strlen, а не sizeof: отправляем только реальные символы, не весь буфер
    write(fd, buffer, strlen(buffer));

    // read(fd, buf, size) — получает ответ от сервера
    //   fd            — тот же дескриптор соединения
    //   buffer        — ПЕРЕЗАПИСЫВАЕТ тот же буфер поверх отправленного
    //   sizeof(buffer) — максимум байт для чтения
    // Возвращает фактическое число прочитанных байт (может быть меньше размера
    // буфера)
    int n = read(fd, buffer, sizeof(buffer));

    // printf с форматом %.*s — выводит ровно n байт из buffer
    //   %.*s — звёздочка берёт точность из аргумента n
    // Важно: без \0 в конце буфера это единственный безопасный способ вывода
    printf("Эхо от сервера: %.*s", n, buffer);
  }
  // Цикл завершается когда пользователь нажмёт Ctrl+D (EOF) или Ctrl+C
  // (SIGTERM)

  // close(fd) — закрывает сокет и освобождает дескриптор
  // Сервер получит EOF на своём read() и корректно завершит дочерний процесс
  close(fd);

  return 0;  // нормальное завершение программы
}
