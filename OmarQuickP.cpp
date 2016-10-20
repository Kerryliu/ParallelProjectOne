#include <iostream> //io
#include <vector>   //vector
#include <stdlib.h> //rand
#include <fstream>  //file rw
#include <chrono>   //clock
#include <numeric>  //average
#include <pthread.h> //black magic

using namespace std;

void quicksort(vector<int>& vect, int left, int right);
void* qsp(void* data);

int numMaxThreads = 0;
int numThreads = 0;

struct potato{
  vector<int>* vec;
  int left;
  int right;
};
int main() {
  //User defined vector size and number of pthreads
  int size = 0;
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
    cin >> numMaxThreads;
    if(numMaxThreads > 0) {
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
  file.open("unsortedArray.txt");
  for(const int& i : vect) {
    file << i << endl;
  }
  file.close();
  
  //Do multiple quicksorts depending on size, then get average time:
  int repetitions = 1;
  vector<int> meow(repetitions);
  for(int i = 0; i < repetitions; i++) {
    auto t1 = chrono::high_resolution_clock::now();
    pthread_t first;
    potato data;
    data.vec = &vect;
    data.left =0;
    data.right = vect.size()-1;
    numThreads = 1;
    pthread_create(&first,NULL,&qsp,&data);
    while(numThreads > 0)
    {
    }
    auto t2 = chrono::high_resolution_clock::now();
    int difference = chrono::duration_cast<chrono::microseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Quicksort average duration: " << average << " microseconds" << endl;

  //write sorted vector to file
  file.open("sortedArray.txt");
  for(const int& i : vect) {
    file << i << endl;
  }
  file.close();
  return 0;
}

void* qsp(void* data) {
  potato *tom = (struct potato*) data;
  quicksort(*tom->vec, tom->left, tom->right);
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
 
  if (left - j > 1000 && numThreads < numMaxThreads) {
    potato temp;
    temp.vec = &vect;
    temp.left = left;
    temp.right = j;
    numThreads++;
    pthread_t ted;
    pthread_create(&ted,NULL,&qsp,&temp);
  }
  else if(left < j)
  {
    quicksort(vect,left,j);
  }
  if (i < right) {
    quicksort(vect, i, right);
  }
  numThreads--;
  pthread_exit(NULL);
}
