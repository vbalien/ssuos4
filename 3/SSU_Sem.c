#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "SSU_Sem.h"

void SSU_Sem_init(SSU_Sem *s, int value) {
  pthread_mutex_init(&s->mtx, NULL);
  pthread_cond_init(&s->cond, NULL);
  s->count = value;
}

void SSU_Sem_down(SSU_Sem *s) {
    pthread_mutex_lock(&s->mtx);
    while(s->count == 0)
        pthread_cond_wait(&s->cond, &s->mtx);
    --(s->count);
    pthread_mutex_unlock(&s->mtx);
}

void SSU_Sem_up(SSU_Sem *s) {
    pthread_mutex_lock(&s->mtx);
    ++(s->count);
    pthread_mutex_unlock(&s->mtx);
    pthread_cond_signal(&s->cond);
}
