#include <iostream> //io
#include <vector>   //vector
#include <stdlib.h> //rand
#include <fstream>  //file rw
#include <chrono>   //clock
#include <numeric>  //average
#include <pthread.h> //black magic
#include<iterator>

using namespace std;

void quicksort(vector<int>& vect, int left, int right);
void* qsp(void* data);

int main() {
  //User defined vector size and number of pthreads
  int size = 0;
  int numThreads = 0;
  int repetitions = 0;
  //Make sure input > 0
  while(1) {
    cout << "Enter positive vector size: ";
    cin >> size;
    if(size == 1) {
      cout << "...?";
    } else if(size > 0) {
      break;
    }
    cout << endl;
  }
  while(1) {
    cout << "Enter positive number of pthreads: ";
    cin >> numThreads;
    if(numThreads > 0) {
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

  //Generate random numbers 0~size
  vector<int> vect(size);
  for(int& i : vect) {
    i = rand() % size;
  }

  ofstream file;
  //write unsorted vector to file
  file.open("qspUnsortedArray.txt");
  copy(vect.begin(), vect.end(), ostream_iterator<int>(file, "\n"));
  file.close();
  
  //Do multiple quicksorts depending on size, then get average time:
  vector<int> meow(repetitions);
  for(int i = 0; i < repetitions; i++) {
    auto t1 = chrono::high_resolution_clock::now();
    pthread_t threads[numThreads];
    for(pthread_t& i : threads) {
      pthread_create(&i, NULL, &qsp, &vect);
    }
    for(pthread_t& i : threads) {
      pthread_join(i, NULL);
    }
    auto t2 = chrono::high_resolution_clock::now();
    int difference = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Quicksort average duration: " << average << " milliseconds" << endl;

  //write sorted vector to file
  file.open("qspSortedArray.txt");
  copy(vect.begin(), vect.end(), ostream_iterator<int>(file, "\n"));
  return 0;
}

void* qsp(void* data) {
  vector<int>* vect = (vector<int>*) data;
  int size = vect->size();
  quicksort(*vect, 0, size - 1);
  //pthread_exit(NULL);
}

void quicksort(vector<int>& vect, int left, int right) {
  int i = left;
  int j = right;
  int pivot = vect[(i + j)/2];

  while(i <= j) {
    while(vect[i] < pivot) {
      i++;
    }
    while(vect[j] > pivot) {
      j--;
    }
    if (i <= j) {
      swap(vect.at(i), vect.at(j));
      i++;
      j--;
    }
  }

  if (left < j) {
    quicksort(vect, left, j);
  }
  if (i < right) {
    quicksort(vect, i, right);
  }
}
