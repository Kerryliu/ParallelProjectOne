#include <iostream> //io
#include <vector>   //vector
#include <stdlib.h> //rand
#include <fstream>  //file rw
#include <chrono>   //clock
#include <numeric>  //average

using namespace std;

void bitonicMerge(vector<int>& vect, int low, int elements, bool direction);
void bitonicSort(vector<int>& vect, int low, int elements, bool direction);

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
  for(const int& i : vect) {
    file << i << endl;
  }
  file.close();

  //Do multiple quicksorts depending on size, then get average time:
  vector<int> meow(repetitions);
  for(int i = 0; i < repetitions; i++) {
    auto t1 = chrono::high_resolution_clock::now();
    bitonicSort(vect, 0, size, true);
    auto t2 = chrono::high_resolution_clock::now();
    int difference = chrono::duration_cast<chrono::microseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
  cout << "Bitonic sort average duration: " << average << " microseconds" << endl;

  //write sorted vector to file
  file.open("bssSortedArray.txt");
  for(const int& i : vect) {
    file << i << endl;
  }
  file.close();

  return 0;
}

void bitonicMerge(vector<int>& vect, int low, int elements, bool direction) {
  if(elements > 1) {
    int j = elements/2;
    for(int i = low; i < low+j; i++) {
      if(direction == (vect[i] > vect[i+j])) {
        swap(vect.at(i), vect.at(i+j));
      }
    }
    bitonicMerge(vect, low, j, direction);
    bitonicMerge(vect, low+j, j, direction);
  }
}

void bitonicSort(vector<int>& vect, int low, int elements, bool direction) {
  if(elements > 1) {
    int j = elements/2;
    //Accending half
    bitonicSort(vect, low, j, true);
    //Decending half
    bitonicSort(vect, low+j, j, false);
    //Now put everything into accending
    bitonicMerge(vect, low, elements, direction);
  }
}
