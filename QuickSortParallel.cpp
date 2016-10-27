#include <iostream>
#include <pthread.h>
#include <vector>
#include <fstream>
#include <chrono>
#include <numeric>  //average
#include<iterator>

int numthread = 0;
pthread_mutex_t lock;
using namespace std;
vector<int> vect;

void quicksort(void* value);
void* qsp(void* value);
struct input {
	vector<int>* vect;
	int left;
	int right;
	input(vector<int>* v, int l, int r) { vect = v;left = l;right = r; }
};

int main()
{
  int size = 0;
  int numthreads = 0;
  int repetitions = 0;

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
    cin >> numthreads;
    if(numthreads > 0) {
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
	fstream myfile;
	myfile.open("input.txt", ios::out);
  copy(vect.begin(), vect.end(), ostream_iterator<int>(myfile, "\n"));
	myfile.close();

	pthread_mutex_init(&lock,NULL);
	vector<pthread_t> threadpool(numthreads);
	vector<input> inputs;
	vector<int> index(2 * numthreads);

	for (int i = 0; i < numthreads;i++)
	{
		index[i] = i * (size / numthreads);
		index[i + numthreads] = ((i + 1) * (size / numthreads) - 1);
		inputs.push_back(input(&vect, index[i], index[i+numthreads]));
	}

  vector<int> meow(repetitions);
  vector<int> final(size);
  for(int i = 0; i < repetitions; i++) {
    auto t1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < numthreads;i++)
    {
      pthread_create(&threadpool[i], NULL, qsp, &inputs[i]);
    }
    for (int i = 0; i < numthreads;i++)
    {
      pthread_join(threadpool[i], NULL);
    }
    int hold = 0;
    int min;
    for (int i = 0; i < final.size();i++)
    {
      min = vect.size()+10;
      for (int j = 0; j < numthreads;j++)
      {
        if (index[j] > index[j + numthreads]) continue;
        if (min > vect[index[j]] && index[j] <= index[j+numthreads])
        {
          min = vect[index[j]];
          hold = j;
        }
      }
      if (hold == -1)
        break;
      final[i] = min;
      index[hold]++;
    }
    auto t2 = chrono::high_resolution_clock::now();
    int difference = chrono::duration_cast<chrono::milliseconds>(t2-t1).count();
    meow[i] = difference;
  }
  double average = accumulate(meow.begin(), meow.end(), 0.0)/ meow.size();
	cout << "Quicksort average duration: " << average << " milliseconds" << endl;

	myfile.open("output.txt", ios::out);
  copy(final.begin(), final.end(), ostream_iterator<int>(myfile, "\n"));
	myfile.close();

	return 0;
}
void* qsp(void* values)
{
	quicksort(values);
	return NULL;
}
void quicksort(void* values) {
	input* data = (input*)values;
	vector<int>& vecRef = *data->vect;
	int i = data->left;
	int j = data->right;
	int pivot = vecRef[(i + j) / 2];

	while (i <= j) {
		while (vecRef[i] < pivot) {
			i++;
		}
		while (vecRef[j] > pivot) {
			j--;
		}
		if (i <= j) {
			swap(vecRef.at(i), vecRef.at(j));
			i++;
			j--;
		}
	}
	if (data->left < j)
	{
		input potato(&vecRef, data->left, j);
		quicksort((void*)&potato);
	}
	if (i < data->right) {
		input potato(&vecRef, i, data->right);
		quicksort((void*)&potato);
	}
}
