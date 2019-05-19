#include "lib.h"
#include "types.h"

int data = 0;

int uEntry(void) {
    /*
    int i = 4;
    int ret = 0;
    sem_t sem;
    printf("Father Process: Semaphore Initializing.\n");
    ret = sem_init(&sem, 2);
    if (ret == -1) {
        printf("Father Process: Semaphore Initializing Failed.\n");
        exit();
    }
    int ppid = getpid();
    printf("ppid = %d\n", ppid);
    printf("ret = %d, sem = %d\n", ret, sem);

    ret = fork();
    if (ret == 0) {
        int pid = getpid();
        printf("pid = %d\n", pid);
        while( i != 0) {
            i --;
            printf("Child Process: Semaphore Waiting.\n");
            sem_wait(&sem);
            printf("Child Process: In Critical Area.\n");
        }
        printf("Child Process: Semaphore Destroying.\n");
        sem_destroy(&sem);
        exit();
    }
    else if (ret != -1) {
        while( i != 0) {
            i --;
            printf("Father Process: Sleeping.\n");
            sleep(128);
            printf("Father Process: Semaphore Posting.\n");
            sem_post(&sem);
        }
        printf("Father Process: Semaphore Destroying.\n");
        sem_destroy(&sem);
        exit();
    }*/
    
    int ret;
    int i;
    //int p;
    printf("ppid = %d\n", getpid());
    //scanf("%x", &p);
    //while(1);
    for(i = 0; i < 6; ++i){
        ret = fork();
        //printf("i = %d, ret = %d\n", i, ret);
        //canf("%d", &p);
        if(ret == -1){
            printf("error!\n");
            return 0;
        }
        if(ret == 0){
            break;
        }
        printf("i = %d, pid = %d\n", i, ret);
    }
    while(1);

    return 0;
}
