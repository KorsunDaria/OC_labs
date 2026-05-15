# Консольные команды — все 7 лабораторных по ОС

---

## Тема 1. Компиляция, сборка, запуск

```bash
gcc hello.c -o hello                              # компилировать в бинарник
gcc -E hello.c -o hello.i                         # только препроцессинг
gcc -S hello.c -o hello.s                         # до ассемблера
gcc -c hello.c -o hello.o                         # до объектного файла
gcc -Wall -O2 hello.c -o hello                    # предупреждения + оптимизация
gcc -fPIC -c mylib.c -o mylib.o                   # позиционно-независимый код
gcc -shared -o libmylib.so mylib.o                # создать .so библиотеку
gcc hello.o -L. -lmylib -o hello                  # слинковать со статической
gcc hello.o -Wl,-rpath,. -lmylib -o hello         # вшить путь к .so
ar rcs libmylib.a foo.o bar.o                     # создать .a архив
nm hello.o                                        # таблица символов
nm -u hello                                       # только unresolved символы
ldd ./hello                                       # зависимости от .so
readelf -S hello                                  # секции ELF-файла
readelf -d hello                                  # динамические зависимости
readelf -l hello                                  # сегменты ELF
readelf --syms hello                              # все символы ELF
objdump -d hello                                  # дизассемблировать код
objdump -s -j .rodata hello                       # дамп секции .rodata
file hello                                        # тип файла
size hello                                        # размеры .text/.data/.bss
```

---

## Тема 2. Системные вызовы

```bash
strace ./hello                                    # все syscall'ы процесса
strace -e write ./hello                           # фильтр по syscall
strace -c ./hello                                 # статистика syscall'ов
strace -p 1234                                    # attach к процессу
strace -o out.txt ./hello                         # вывод в файл
strace -f ./hello                                 # следить за дочерними
strace -e trace=file ls                           # только файловые syscall'ы
strace -e trace=network curl kernel.org           # только сетевые syscall'ы
ltrace ./hello                                    # вызовы libc-функций
ausyscall write                                   # номер syscall по имени
ausyscall 1                                       # имя по номеру
perf stat ./hello                                 # счётчики производительности
```

---

## Тема 3. Файлы и файловые системы

```bash
ls -li /etc/passwd                                # inode + счётчик ссылок
stat hello.c                                      # метаданные файла
stat -L symlink.txt                               # метаданные цели симлинка
ln file.txt hardlink.txt                          # создать жёсткую ссылку
ln -s /path/to/file sym.txt                       # создать символьную ссылку
readlink sym.txt                                  # путь внутри симлинка
readlink -f sym.txt                               # полный путь рекурсивно
find . -type f -name "*.c"                        # найти файлы по маске
find . -inum 123456                               # найти по inode
find / -perm -4000 2>/dev/null                    # все SUID-файлы
find . -links +1 -type f                          # файлы с хард-линками
chmod 644 file.txt                                # изменить права доступа
chmod u+x script.sh                               # добавить execute-бит
chown user:group file.txt                         # изменить владельца
mkdir mydir                                       # создать директорию
rmdir mydir                                       # удалить директорию
touch myfile                                      # создать пустой файл
rm myfile                                         # удалить файл
cp src.txt dst.txt                                # скопировать файл
xxd /proc/$$/pagemap | head -5                    # дамп pagemap
cat /proc/$$/maps                                 # карта памяти процесса
```

---

## Тема 4. Адресное пространство процесса

```bash
cat /proc/1234/maps                               # регионы адресного пространства
cat /proc/1234/smaps                              # регионы + RSS по каждому
cat /proc/1234/smaps_rollup                       # сводка по всей памяти
grep -E "\[heap\]|\[stack\]" /proc/1234/maps      # найти heap и stack
watch -n 0.5 cat /proc/1234/maps                  # мониторинг в реальном времени
pmap 1234                                         # карта памяти красиво
pmap -x 1234                                      # карта + RSS/dirty
cat /proc/1234/status                             # статус + Vm* метрики
grep -E "VmRSS|VmSize|VmStk" /proc/1234/status   # размеры памяти
valgrind --leak-check=full ./prog                 # детектор утечек
ulimit -a                                         # все лимиты процесса
ulimit -s unlimited                               # снять лимит стека
free -h                                           # использование RAM/swap
env                                               # все переменные окружения
printenv PATH                                     # одна переменная окружения
cat /proc/$$/environ | tr '\0' '\n'               # окружение текущего процесса
```

---

## Тема 5. Создание и завершение процесса

```bash
ps aux                                            # все процессы системы
ps -ef                                            # все процессы + PPID
ps axjf                                           # дерево в ASCII
ps -o pid,ppid,stat,cmd                           # выбрать колонки
ps -eo pid,ppid,stat,cmd | awk '$3 ~ /Z/'         # найти зомби
pstree -p                                         # дерево процессов с PID
pstree -p 1234                                    # поддерево процесса
pstree -s 1234                                    # цепочка родителей
pgrep bash                                        # PID по имени
pgrep -a python                                   # PID + командная строка
pgrep -P 1234                                     # дочерние процессы
pidof bash                                        # PID всех bash
cat /proc/1234/status                             # полный статус процесса
grep -E "State|Pid|PPid|Uid" /proc/1234/status    # ключевые поля
cat /proc/1234/cmdline | tr '\0' ' '              # аргументы процесса
readlink /proc/1234/exe                           # путь к бинарнику
readlink /proc/1234/cwd                           # рабочая директория
cat /proc/1234/wchan                              # в каком syscall спит
cat /proc/1234/syscall                            # текущий syscall
watch -n 1 "ps -o pid,ppid,stat,cmd --ppid 1234"  # мониторинг дочерних
ls /proc | grep '^[0-9]' | wc -l                  # количество процессов
```

---

## Тема 6. Межпроцессное взаимодействие (IPC)

```bash
ipcs -a                                           # все IPC-объекты
ipcs -m                                           # разделяемая память
ipcrm -m 12345                                    # удалить shared memory
ss -x                                             # Unix Domain сокеты
ss -xlp                                           # сокеты + PID процесса
lsof -i                                           # все сетевые сокеты
lsof -p 1234                                      # открытые файлы процесса
kill -SIGUSR1 1234                                # послать SIGUSR1
kill -SIGUSR2 1234                                # послать SIGUSR2
kill -SIGINT 1234                                 # послать SIGINT
kill -9 1234                                      # принудительно завершить
mkfifo /tmp/mypipe                                # создать именованный pipe
nc -U /tmp/my.sock                                # клиент Unix-сокета
nc -lU /tmp/my.sock                               # сервер Unix-сокета
socat - UNIX-CONNECT:/tmp/echo.sock               # подключиться к сокету
socat UNIX-LISTEN:/tmp/echo.sock,fork EXEC:cat    # эхо-сервер на сокете
strace -e trace=pipe,read,write ./prog            # трассировка pipe-операций
```

---

## Тема 7. Пользователи и управление правами. SUID

```bash
id                                                # UID, GID, группы
id root                                           # UID другого пользователя
whoami                                            # имя пользователя по EUID
ls -l /usr/bin/passwd                             # видеть SUID-бит (s)
chmod u+s ./myprog                                # установить SUID-бит
chmod 4755 ./myprog                               # SUID числом
chmod u-s ./myprog                                # снять SUID-бит
find / -perm -4000 -type f 2>/dev/null            # все SUID-файлы
find / -perm -2000 -type f 2>/dev/null            # все SGID-файлы
sudo -u www-data id                               # запуск от другого юзера
su - otheruser                                    # shell другого пользователя
grep "^Uid\|^Gid" /proc/$(pgrep passwd)/status    # RUID/EUID процесса
getcap /usr/bin/ping                              # capabilities файла
setcap cap_net_raw+ep ./myping                    # задать capability
getfacl file.txt                                  # расширенные ACL файла
setfacl -m u:bob:r file.txt                       # добавить ACL-правило
```
