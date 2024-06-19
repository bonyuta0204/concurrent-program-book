#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int count = 0;

void *some_func(void *arg) {
  for (int i = 0; i < 10000000; i++) {
    if (pthread_mutex_lock(&mut) != 0) {
      perror("pthread_mutex_lock");
      exit(-1);
    }
    count = count + 1;
    if (pthread_mutex_unlock(&mut) != 0) {
      perror("pthread_mutex_unlock");
      exit(-1);
    }
  }
  printf("count: %d", count);

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t th1, th2;

  if (pthread_create(&th1, NULL, some_func, NULL) != 0) {
    perror("pthread_create");
    return -1;
  }

  if (pthread_create(&th2, NULL, some_func, NULL) != 0) {
    perror("pthread_create");
    return -1;
  }

  // スレッドの終了を待機
  if (pthread_join(th1, NULL) != 0) {
    perror("pthread_join");
    return -1;
  }

  if (pthread_join(th2, NULL) != 0) {
    perror("pthread_join");
    return -1;
  }

  if (pthread_mutex_destroy(&mut) != 0) {
    perror("pthread_mutex_destroy");
  }
}
