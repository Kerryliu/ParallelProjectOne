#include <iostream>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <chrono>
#include <numeric>

using namespace std;

void genValues(vector<vector<double> >& matrixA, vector<double>& b);
void* threading(void* data);
void gaussianElimination(vector<vector<double> >& matrixA, vector<double>& b, vector<double>& y, int numThreads);

int numThreads = 0;
pthread_barrier_t barrier;

struct potato{
  vector< vector<double> >* matrix; //This can probably be a global, but this allows more flexibility
  int threadID;
};

int main() {
  int size = 0;
  int repetitions = 0;

  //Get user inputs
  while(1) {
    cout << "Enter positive matrix size: ";
    cin >> size;
    if(size == 1) {
      cout << "...?";
    } else if(size > 0) {
      break;
    }
    cout << endl;
  }
  while(1) {
    cout << "Enter positive number of pthreads that is less than matrix size: ";
    cin >> numThreads;
    if(numThreads > 0 && numThreads < size) {
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

  //Define matrixes and vectors
  vector<double> b(size);
  vector<vector<double> > matrixA(size, vector<double>(size));
  vector<double> y(b.size());
  genValues(matrixA, b);

  //Write random A matrix and b vector to file
  ofstream file;
  file.open("MatrixA.txt");
  for(const auto& i : matrixA) {
    for(const double& j : i) {
      file << j << " ";
    }
    file << endl;
  }
  file.close();
  file.open("Vectorb.txt");
  for(const double&i : b) {
    file << i << endl;
  }
  file.close();

  //Run through algorithm x times, and get average runtime
  vector<int> meow(repetitions);
  for(int i = 0; i < repetitions; i++) {
    auto t1 = chrono::high_resolution_clock::now();
    gaussianElimination(matrixA, b, y, numThreads);
    auto t2 = chrono::high_resolution_clock::now();
    int difference = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Gaussian average duration: " << average << " milliseconds" << endl;

  //Write calculated y vector to file
  file.open("Vectory.txt"); //hehe... if it succeeds, it's a "vectory"
  for(double&i : y) {
    file << i << endl;
  }
  file.close();
  return 0;
}

void* threading(void* data) {
  struct potato *stuff = (struct potato*) data;
  vector<vector<double> >* matrix = stuff->matrix;
  int offset = stuff->threadID;
  int w = matrix->size();
  for(int k = 0; k < w; k++) { //Outer loop
    pthread_barrier_wait(&barrier);
    for(int j = k+1+offset; j < w; j += numThreads) {
      (*matrix)[k][j] /= (*matrix)[k][k]; //Division step
      for(int i = k+1; i < w; i++) {
        (*matrix)[i][j] -= (*matrix)[i][k]*(*matrix)[k][j]; //Eliminination step
      }
    }
  }
}

void gaussianElimination(vector<vector<double> >& matrixA, vector<double>& b, vector<double>& y, int numThreads) {
  int w = matrixA.size();
  pthread_t threads[numThreads];
  struct potato args[numThreads];

  pthread_barrier_init(&barrier, NULL, numThreads);
  for(int i = 0; i < numThreads; i++) {
    args[i] = {&matrixA, i};
    pthread_create(&threads[i], NULL, &threading, (void*) &args[i]);
  }
  for(pthread_t& i : threads) {
    pthread_join(i, NULL);
  }
  for(int k = 0; k < w; k++) { //Second pass of outer loop
    y[k] = b[k]/matrixA[k][k];
    matrixA[k][k] = 1;
    for(int i = k+1; i < w; i++) {
      b[i] -= matrixA[i][k]*y[k];
      matrixA[i][k] = 0;
    }
  }
  for(int k = w-1; k>= 0; k--) { //Back substitution
    b[k] = y[k];
    for(int i = k-1; i >= 0; i--) {
      y[i] -= b[k]*matrixA[i][k];
    }
  }
}

//No time to parallize this.  Not because I'm lazy or anything
void genValues(vector<vector<double> >& matrixA, vector<double>& b) {
  int size = matrixA[0].size();
  for(auto& i : matrixA) {
    for(double& j : i) {
      j = rand() % (size*10) + 1;
    }
  }
  size = b.size();
  for(double& i : b) {
     i = rand() % (size*10) + 1;
  }
}
