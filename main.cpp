#include<iostream>
#include<fstream>
#include<unistd.h>
#include<cstring>
#include<math.h>
#include<algorithm>

#define START_K 10
#define END_K 30
#define MAX_COUNT 10
#define MAX_SEC 360

using namespace std;

void random(int* ptr, int k, int seed){
	srand(seed);
	for(int a = 0; a < (1 << k); a++){
		ptr[a] = (rand() >> 21);	//	MOD 1024(2^10) is slightly more faster than MOD 1000.
	}
	return;
}

void almost_sort(int* ptr, int k, int seed){
	srand(seed);
	for(int a = 0; a < (1 << k); a++){
		ptr[a] = a+1;
	}
	for(int a = 0; a < 100; a++){
		int idx = (rand() >> (31 - k));
		while(ptr[idx] != idx+1){
			idx = (rand() >> (31 - k));
		}
		ptr[idx] = (rand() >> 21);	//	MOD 1024(2^10) is slightly more faster than MOD 1000.
	}
	return;
}

bool insertion_sort(int* array, int size, clock_t start_t){
	for(int a = 0; a < size; a++){
		if(((clock()-start_t)/CLOCKS_PER_SEC) > MAX_SEC){
			return true;
		}
		int b;
		for(b = a-1; b >= 0; b--){
			if(array[a] >= array[b]){
				break;
			}
		}
		int buf = array[a];
		memmove(array+b+2, array+b+1, (a-b-1)*sizeof(int));
		array[b+1] = buf;
	}
	return false;
}

bool merge_sort(int* array, int size, clock_t start_t){
	if(size == 1){
		return false;
	}
	if(((clock()-start_t)/CLOCKS_PER_SEC) > MAX_SEC){
		return true;
	}
	if(merge_sort(array, (size >> 1), start_t) || merge_sort(array + (size >> 1), (size >> 1), start_t)){
		return true;
	}
	int* sorted = new int[size];
	int idx1 = 0;
	int idx2 = (size >> 1);
	for(int a = 0; a < size; a++){
		if(idx1 == (size >> 1)){
			for(; a < size; a++){
				sorted[a] = array[idx2];
				idx2++;
			}
			break;
		}
		if(idx2 == size){
			for(; a < size; a++){
				sorted[a] = array[idx1];
				idx1++;
			}
			break;
		}
		if(array[idx2] < array[idx1]){
			sorted[a] = array[idx2];
			idx2++;
		}
		else{
			sorted[a] = array[idx1];
			idx1++;
		}
	}
	memcpy(array, sorted, sizeof(int)*size);
	delete [] sorted;
	return false;
}

bool quick_sort(int* array, int size, clock_t start_t){
	if(((clock() - start_t)/CLOCKS_PER_SEC) > MAX_SEC){
		return true;
	}
	if(size == 1 || size == 0){
		return false;
	}
	int* sorted = new int[size];
	int idx = rand()%size;
	int idx1 = 0;
	int idx2 = size-1;
	for(int a = 0; a < size; a++){
		if(array[a] < array[idx]){
			sorted[idx1] = array[a];
			idx1++;
		}
		else if(array[a] > array[idx]){
			sorted[idx2] = array[a];
			idx2--;
		}
		else{
			continue;
		}
	}
	for(int a = idx1; a <= idx2; a++){
		sorted[a] = array[idx];
	}
	memcpy(array, sorted, sizeof(int)*size);
	delete [] sorted;
	if(quick_sort(array, idx1, start_t) || quick_sort(array+idx2+1, size-idx2-1, start_t)){
		return true;
	}
	return false;
}
bool counting_sort(int* array, int size, clock_t start_t){
	int* counter = new int[size+1];
	memset(counter, 0, sizeof(int)*(size+1));
	for(int a = 0; a < size; a++){
		counter[array[a]] += 1;
	}
	int top = 0;
	for(int a = 0; a <= size; a++){
		if(((clock() - start_t)/CLOCKS_PER_SEC) > MAX_SEC){
			return true;
		}
		for(int b = 0; b < counter[a]; b++){
			array[top] = a;
			top++;
		}
		if(top == size){
			break;
		}
	}
	delete [] counter;
	return false;
}
bool std_sort(int* array, int size, clock_t start_t){
	sort(array, array+size);
	return false;
}
int check(int* array, int size){
	for(int idx = 1; idx < size; idx++){
		if(array[idx] < array[idx-1]){
			return 1;
		}
	}
	return 0;
}

int main(){
	ofstream csv_file;
	double time_array[END_K-START_K+1][10][MAX_COUNT+1];
	int seed[END_K-START_K+1][MAX_COUNT];
	for(int a = 0; a < END_K-START_K+1; a++){
		for(int b = 0; b < 10; b++){
			for(int c = 0; c < MAX_COUNT; c++){
				time_array[a][b][c] = 0;
			}
		}
	}
	for(int k = START_K; k <= END_K; k++){
		srand(clock());
		for(int count = 0; count < MAX_COUNT; count++){
			seed[k-START_K][count] = rand();
		}
		
		int* array = new int[(1 << k)];
		clock_t timer;
		//
		//	Initialize Complete.
		//

		for(int count = 0; count < MAX_COUNT; count++){
			random(array, k, seed[k-START_K][count]);
			timer = clock();
			if(insertion_sort(array, (1 << k), timer)){
				time_array[k-START_K][0][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Insertion sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][0][count] = timer;
		}
		
		//
		//	Insertion sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			random(array, k, seed[k-START_K][count]);
			timer = clock();
			if(merge_sort(array, (1 << k), timer)){
				time_array[k-START_K][1][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Merge sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][1][count] = timer;
		}

		//
		//	Merge sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			random(array, k, seed[k-START_K][count]);
			timer = clock();
			if(quick_sort(array, (1 << k), timer)){
				time_array[k-START_K][2][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Quick sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][2][count] = timer;
		}

		//
		//	Quick sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			random(array, k, seed[k-START_K][count]);
			timer = clock();
			if(counting_sort(array, (1 << k), timer)){
				time_array[k-START_K][3][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Counting sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][3][count] = timer;
		}

		//
		//	Counting sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			random(array, k, seed[k-START_K][count]);
			timer = clock();
			if(std_sort(array, (1 << k), timer)){
				time_array[k-START_K][4][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "STD sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][4][count] = timer;
		}

		//
		//	g++ std sort complete.
		//
		//**************************************
		//
		//		random
		//
		//**************************************
		
		for(int count = 0; count < MAX_COUNT; count++){
			almost_sort(array, k, seed[k-START_K][count]);
			timer = clock();
			if(insertion_sort(array, (1 << k), timer)){
				time_array[k-START_K][5][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Insertion sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][5][count] = timer;
		}

		//
		//	Insertion sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			almost_sort(array, k, seed[k-START_K][count]);
			timer = clock();
			if(merge_sort(array, (1 << k), timer)){
				time_array[k-START_K][6][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Merge sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][6][count] = timer;
		}

		//
		//	Merge sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			almost_sort(array, k, seed[k-START_K][count]);
			timer = clock();
			if(quick_sort(array, (1 << k), timer)){
				time_array[k-START_K][7][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Quick sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][7][count] = timer;
		}

		//
		//	Quick sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			almost_sort(array, k, seed[k-START_K][count]);
			timer = clock();
			if(counting_sort(array, (1 << k), timer)){
				time_array[k-START_K][8][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "Counting sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][8][count] = timer;
		}

		//
		//	Counting sort complete.
		//
		
		for(int count = 0; count < MAX_COUNT; count++){
			almost_sort(array, k, seed[k-START_K][count]);
			timer = clock();
			if(std_sort(array, (1 << k), timer)){
				time_array[k-START_K][9][0] = -1;
				break;
			}
			timer = clock() - timer;
			if(check(array, (1 << k))){
				cout << "STD sort ERROR" << endl;
				exit(1);
			}
			time_array[k-START_K][9][count] = timer;
		}

		//
		//	g++ std sort complete.
		//
		
		delete [] array;
		csv_file.open("./resultso3.csv", ofstream::app);
		for(int a = 0; a < MAX_COUNT; a++){
			csv_file << "," << seed[k-START_K][a];
		}
		csv_file << "\n";
		cout << k << "\t";
		for(int a = 0; a < 10; a++){
			if(time_array[k-START_K][a][0] != -1){
				double buf = 0;
				for(int b = 0; b < MAX_COUNT; b++){
					buf += time_array[k-START_K][a][b];
					csv_file << "," << time_array[k-START_K][a][b]/CLOCKS_PER_SEC;
				}
				time_array[k-START_K][a][MAX_COUNT] = buf/CLOCKS_PER_SEC/MAX_COUNT;
				csv_file << "," << buf/CLOCKS_PER_SEC/MAX_COUNT;
				cout << buf/CLOCKS_PER_SEC/MAX_COUNT << "\t";
			}
			else{
				time_array[k-START_K][a][MAX_COUNT] = -1;
				cout << -1 << "\t";
			}
			csv_file << "\n";
		}
		csv_file << "\n";
		cout << endl;
		csv_file.close();
	}
	csv_file.open("./resultso3.csv", ofstream::app);
	for(int k = START_K; k <= END_K; k++){
		csv_file << k << ",";
		for(int a = 0; a < 5; a++){
			csv_file << time_array[k-START_K][a][MAX_COUNT] << ",";
		}
		csv_file << "," << k << ",";
		for(int a = 5; a < 10; a++){
			csv_file << time_array[k-START_K][a][MAX_COUNT] << ",";
		}
		csv_file << "\n";
	}
	csv_file.close();
	return 0;
}
