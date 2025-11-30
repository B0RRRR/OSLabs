#include "ipc_shm.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>


struct shm* ipc_shm_init(void) {
    int fd = open(SHM_PATH, O_RDWR | O_CREAT, 0600);
    if (fd == -1) {
        fprintf(stderr, "open(%s) failed: %s\n", SHM_PATH, strerror(errno));
        return NULL;
    }

    if (ftruncate(fd, SHM_SIZE) == -1) {
        fprintf(stderr, "ftruncate failed: %s\n", strerror(errno));
        close(fd);
        return NULL;
    }

    struct shm *area = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (area == MAP_FAILED) {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        close(fd);
        return NULL;
    }
    close(fd);

    area->flag = 0;
    memset(area->buf, 0, sizeof(area->buf));
    return area;
}

void ipc_shm_cleanup(struct shm *area) {
    if (!area) return;
    munmap(area, SHM_SIZE);
    unlink(SHM_PATH);
}
