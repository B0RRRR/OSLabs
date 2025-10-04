#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int main() {
    char filename[256];
    // fd для передачи данных от родителя к дочернему процессу
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Failed to create pipe\n");
        return -1;
    }

    printf("Filename:\n");
    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Incorrect file name\n");
        return -1;
    }

    int fd_file = open(filename, O_RDONLY);
    if (fd_file == -1) {
        fprintf(stderr, "Failed to open file\n");
        return -1;
    }

    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        close(fd_file);
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (pid == 0) {
        // Закрываем канал для чтения
        close(pipefd[0]);

        // Перенаправляем ввод в файл (чтобы читал из файла)
        if (dup2(fd_file, STDIN_FILENO) == -1) {
            fprintf(stderr, "dup2(fd_file->stdin) failed:\n");
            return -1;
        }

        // Перенаправляем вывод дочернего процесса в канал
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            fprintf(stderr, "dup2(pipe_write->stdout) failed:\n");
            return -1;
        }

        close(fd_file);
        close(pipefd[1]);

        // Перенаправляем процесс в child
        char *argv[] = {"./child", NULL};
        execv(argv[0], argv);

        fprintf(stderr, "execv failed\n");
        return -1;
    } else {
        // Закрываем канал для записи
        close(pipefd[1]);
        close(fd_file);

        // Цикл для того чтобы вывести в терминал каждый ответ
        ssize_t n;
        char buffer[4096];
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            ssize_t out = 0;
            while (out < n) {
                ssize_t w = write(STDOUT_FILENO, buffer + out, n - out);
                if (w == -1) {
                    perror("write");
                    break;
                }
                out += w;
            }
        }
          
    if (n == -1) {
        fprintf(stderr, "Failed read from pipe");
    }

    close(pipefd[0]);
    if (wait(NULL) == -1) {
        perror("wait");
    }

    }

    return 0;
}