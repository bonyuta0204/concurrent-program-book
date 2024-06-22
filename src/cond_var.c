#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

volatile bool ready = false;
char buf[256];

void *producer(void *arg) {
  printf("producer: ");

  fgets(buf, sizeof(buf), stdin);

  pthread_mutex_lock(&mut);

  ready = true;

  if (pthread_cond_broadcast(&cond) != 0) {
    perror("pthread_cond_broadcast");
    exit(-1);
  }

  pthread_mutex_unlock(&mut);

  return NULL;
}

void *consumer(void *arg) {
  pthread_mutex_lock(&mut);

  while (!ready) {

    if (pthread_cond_wait(&cond, &mut) != 0) {
      perror("pthread_cond_wait");
      exit(-1);
    }
  }

  pthread_mutex_unlock(&mut);

  printf("consumer: %s\n", buf);
  return NULL;
}

int main(int argc, char *argv[]) {
  printf("mutex __sid: %ld", mut.__sig);
  printf("mutex __opaque: %s", mut.__opaque);
  // スレッド生成
  pthread_t pr, cn;
  pthread_create(&pr, NULL, producer, NULL);
  pthread_create(&cn, NULL, consumer, NULL);

  // スレッドの終了を待機
  pthread_join(pr, NULL);
  pthread_join(cn, NULL);

  // destroy mutex
  pthread_mutex_destroy(&mut);

  if (pthread_cond_destroy(&cond) != 0) {
    perror("thread_cond_destroy");
    return -1;
  }

  return 0;
}
