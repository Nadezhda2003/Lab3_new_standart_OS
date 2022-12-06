#include <iostream>
#include <Windows.h> 
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
using std::endl;
using std::cin;
using std::cout;
bool* checkArr;
int* arr;
int n;
int markerCount;
std::mutex signal;
std::condition_variable con;
int number = -1;
bool StopThreads() {
	for (int i = 0; i < markerCount; i++) {
		if (checkArr[i]) {
			return false;
		}
	}
	return true;
}
bool check(const std::vector<int>& endOftheThreads, int i) {
	for (int j = 0; j < endOftheThreads.size(); j++) {
		if (endOftheThreads[j] == i) {
			return true;
		}
	}
	return false;
}

int main() {
	return 0;
}