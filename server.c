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

    // ждем, пока клиент запишет данные в разделяемую память
    while (*shm == 0)
        sleep(1);

    // выводим данные из разделяемой памяти
    for (s = shm; *s != '\0'; s++)
        putchar(*s);
    putchar('\n');

    // отсоединяем сегмент разделяемой памяти
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    // удаляем сегмент разделяемой памяти
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
