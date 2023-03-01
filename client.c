#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  // размер разделяемой памяти

int main()
{
    int shmid;      // идентификатор сегмента разделяемой памяти
    key_t key = 1234; // ключ для создания сегмента разделяемой памяти
    char *shm, *s;

    // создаем сегмент разделяемой памяти
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // присоединяем сегмент к нашему пространству данных
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // записываем данные в разделяемую память
    s = shm;
    for (int i = 0; i < 10; i++) {
        *s++ = rand() % 10 + '0';
    }
    *s = '\0';
