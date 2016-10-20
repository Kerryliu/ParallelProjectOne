#include <iostream>
#include <pthread.h>

using namespace std;

void* say_hello(void* data) {
  char* str = (char*) data;
  printf("%s\n", str);
}

int main() {
  pthread_t t1, t2;
  pthread_create(&t1, NULL, &say_hello, (void*) "Hello from thread 1");
  pthread_create(&t2, NULL, &say_hello, (void*) "Hello from thread 2");
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  return 0;
}
