#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t barrier_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond = PTHREAD_COND_INITIALIZER;

void barrier(volatile int *cnt, int max) {

  if (pthread_mutex_lock(&barrier_mut) != 0) {
    perror("mutex lock");
  }

  (*cnt)++;

  if (*cnt == max) {
    if (pthread_cond_broadcast(&barrier_cond) != 0) {
      perror("failed to broadcast");
    }

  } else {
    do {
      if (pthread_cond_wait(&barrier_cond, &barrier_mut) != 0) {
        perror("failed to wait");
      }
    } while (*cnt < max);
  }

  if (pthread_mutex_unlock(&barrier_mut) != 0) {
    perror("Failed to unlock mutex"); exit(-1);
  }
}
