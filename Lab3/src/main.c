#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>


#include "ipc_shm.h"
#include "parser.h"
#include "signals.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct shm *area = ipc_shm_init();
    if (!area) return EXIT_FAILURE;

    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        ipc_shm_cleanup(area);
        return EXIT_FAILURE;
    }

    if (child == 0) {
        /* Дочерний процесс */
        pid_t parent_pid = getppid();
        install_handler(SIGUSR1, child_sigusr1_handler);

        while (1) {
            while (!child_got_signal) pause();
            child_got_signal = 0;

            int f = area->flag;
            if (f == 1) {
                char local[BUF_SIZE];
                strncpy(local, area->buf, BUF_SIZE-1);
                local[BUF_SIZE-1] = '\0';

                double sum;
                int res = sum_floats_in_str(local, &sum);
                if (res == 0)
                    printf("PID %d: сумма = %.6f\n", getpid(), (float)sum);
                else if (res == 1)
                    printf("PID %d: нет чисел\n", getpid());
                else
                    fprintf(stderr, "PID %d: ошибка парсинга\n", getpid());
                fflush(stdout);

                area->flag = 0;
                kill(parent_pid, SIGUSR2);
            } else if (f == 2) break;
        }

        ipc_shm_cleanup(area);
        return EXIT_SUCCESS;
    } else {
        /* Родительский процесс */
        install_handler(SIGUSR2, parent_sigusr2_handler);

        FILE *fin = fopen(argv[1], "r");
        if (!fin) {
            perror("fopen");
            area->flag = 2;
            kill(child, SIGUSR1);
            waitpid(child, NULL, 0);
            ipc_shm_cleanup(area);
            return EXIT_FAILURE;
        }

        char line[BUF_SIZE];
        while (fgets(line, sizeof(line), fin)) {
            while (area->flag != 0) usleep(1000);

            strncpy(area->buf, line, BUF_SIZE-1);
            area->buf[BUF_SIZE-1] = '\0';
            area->flag = 1;
            kill(child, SIGUSR1);

            while (!parent_notified) usleep(1000);
            parent_notified = 0;
        }

        fclose(fin);
        area->flag = 2;
        kill(child, SIGUSR1);
        waitpid(child, NULL, 0);
        ipc_shm_cleanup(area);
    }

    return EXIT_SUCCESS;
}
