#ifndef IPC_SHM_H
#define IPC_SHM_H

#include <stdint.h>

#define SHM_PATH "/tmp/ipc_mmap_file.bin"
#define SHM_SIZE 8192
#define BUF_SIZE (SHM_SIZE - sizeof(int))

struct shm {
    volatile int flag;   // 0 = idle, 1 = data ready, 2 = terminate
    char buf[BUF_SIZE];
};

/* Создаёт и отображает файл в память */
struct shm* ipc_shm_init(void);

/* Освобождает ресурсы */
void ipc_shm_cleanup(struct shm *area);

#endif
