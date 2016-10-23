#include <iostream> //io
#include <vector>   //vector
#include <stdlib.h> //rand
#include <fstream>  //file rw
#include <chrono>   //clock
#include <numeric>  //average
#include<iterator>

using namespace std;

void quicksort(vector<int>& vect, int left, int right);

int main() {
  //User defined vector size
  int size = 0;
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
  file.open("qssUnsortedArray.txt");
  copy(vect.begin(), vect.end(), ostream_iterator<int>(file, "\n"));
  file.close();
  
  //Do multiple quicksorts depending on size, then get average time:
  vector<int> meow(repetitions);
  for(int i = 0; i < repetitions; i++) {
    auto t1 = chrono::high_resolution_clock::now();
    quicksort(vect, 0, size - 1);
    auto t2 = chrono::high_resolution_clock::now();
    int difference = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Quicksort average duration: " << average << " milliseconds" << endl;

  //write sorted vector to file
  file.open("qssSortedArray.txt");
  copy(vect.begin(), vect.end(), ostream_iterator<int>(file, "\n"));
  file.close();
  return 0;
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
