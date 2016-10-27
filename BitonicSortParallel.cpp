#include <iostream> //io
#include <vector>   //vector
#include <stdlib.h> //rand
#include <pthread.h>
#include <fstream>  //file rw
#include <chrono>   //clock
#include <numeric>  //average
#include<iterator>

using namespace std;

void bitonicParallel(vector<int>& vect);
void bitonicSort(vector<int>& vect, int low, int elements, int offset, bool direction);
void bitonicMerge(vector<int>& vect, int low, int elements, int offset, bool direction);
void* threading(void* data);
int numThreads = 0;
pthread_barrier_t barrier;
struct potato{
  vector<int>* vect;
  int threadID;
};

int main() {
  //User defined vector size
  int size = 1;
  int repetitions = 0;
  //Make sure input is a power of 2 and size > 0
  while(1) {
    cout << "Enter positive vector size that is a power of 2: ";
    cin >> size;
    if(size == 1) {
      cout << "...?";
    } else if((size & (size - 1)) == 0 && size > 0) {
      //Cheeky little bit trick to check size is a power of 2
      break;
    }
    cout << endl;
  }
  while(1) {
    cout << "Enter positive number of pthreads that is less than half vector size: ";
    cin >> numThreads;
    if(numThreads < size/2) {
      break;
    }
    cout << endl;
  }
  while(1) {
    cout << "Enter number of repetitions: ";
    cin >> repetitions;
    if(repetitions > 0) {
      break;
    }
    cout << endl;
  }

  vector<int> vect(size);

  //Generate random numbers 0~size
  for(int& i : vect) {
    i = rand() % size;
  }

  ofstream file;
  //write unsorted vector to file
  file.open("bssUnsortedArray.txt");
  copy(vect.begin(), vect.end(), ostream_iterator<int>(file, "\n"));
  file.close();

  //Do multiple quicksorts depending on size, then get average time:
  vector<int> meow(repetitions);
  for(int i = 0; i < repetitions; i++) {
    auto t1 = chrono::high_resolution_clock::now();
    bitonicParallel(vect);
    auto t2 = chrono::high_resolution_clock::now();
    int difference = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Bitonic sort average duration: " << average << " milliseconds" << endl;

  //write sorted vector to file
  file.open("bssSortedArray.txt");
  copy(vect.begin(), vect.end(), ostream_iterator<int>(file, "\n"));
  file.close();

  return 0;
}

void bitonicParallel(vector<int>& vect) {
  int l = vect.size();
  int sectorSize = l/numThreads;
  struct potato args[numThreads];
  pthread_t threads[numThreads];
  pthread_barrier_init(&barrier, NULL, numThreads);
  for(int i = 0; i < numThreads; i++) {
    args[i] = {&vect, i};
    pthread_create(&threads[i], NULL, &threading, (void*) &args[i]);
  }
  for(pthread_t& i : threads) {
    pthread_join(i, NULL);
  }
}

void* threading(void* data) {
  struct potato* stuff = (struct potato*) data;
  vector<int>* vect = stuff->vect;
  int size = vect->size();
  int offset = stuff->threadID;
  bitonicSort(*vect, 0, size, offset, true);
}

void bitonicMerge(vector<int>& vect, int low, int elements, int offset, bool direction) {
  pthread_barrier_wait(&barrier);
  if(elements > 1) {
    int j = elements/2;
    for(int i = low+offset; i < low+j; i+=numThreads) {
      if(direction == (vect[i] > vect[i+j])) {
        swap(vect.at(i), vect.at(i+j));
      }
    }
    bitonicMerge(vect, low, j, offset, direction);
    bitonicMerge(vect, low+j, j, offset, direction);
  }
}

void bitonicSort(vector<int>& vect, int low, int elements, int offset, bool direction) {
  if(elements > 1) {
    int j = elements/2;
    //Accending half
    bitonicSort(vect, low, j, offset, true);
    //Decending half
    bitonicSort(vect, low+j, j, offset, false);
    //Now put everything into accending
    bitonicMerge(vect, low, elements, offset, direction);
  }
}
