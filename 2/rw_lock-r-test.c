#include "rw_lock.h"

void init_rwlock(struct rw_lock *rw) {
  //	Write the code for initializing your read-write lock.
  rw->r_active = rw->w_active = 0;
  rw->r_wait = rw->w_wait = 0;

  pthread_mutex_init(&rw->mtx, NULL);
  pthread_cond_init(&rw->read, NULL);
  pthread_cond_init(&rw->write, NULL);
}

void r_lock(struct rw_lock *rw) {
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->mtx);
  if (rw->w_active) {
    rw->r_wait++;
    while (rw->w_active)
      pthread_cond_wait(&rw->read, &rw->mtx);
    rw->r_wait--;
  }
  rw->r_active++;
  pthread_mutex_unlock(&rw->mtx);
}

void r_unlock(struct rw_lock *rw) {
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->mtx);
  rw->r_active--;
  if (rw->r_active == 0 && rw->w_wait > 0)
    pthread_cond_signal(&rw->write);
  pthread_mutex_unlock(&rw->mtx);
}

void w_lock(struct rw_lock *rw) {
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->mtx);
  if (rw->w_active || rw->r_active > 0) {
    rw->w_wait++;
    while (rw->w_active || rw->r_active > 0)
      pthread_cond_wait(&rw->write, &rw->mtx);
    rw->w_wait--;
  }
  rw->w_active = 1;
  pthread_mutex_unlock(&rw->mtx);
}

void w_unlock(struct rw_lock *rw) {
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->mtx);
  rw->w_active = 0;
  if (rw->r_wait > 0) {
    pthread_cond_broadcast(&rw->read);
  } else if (rw->w_wait > 0) {
    pthread_cond_signal(&rw->write);
  }
  pthread_mutex_unlock(&rw->mtx);
}
