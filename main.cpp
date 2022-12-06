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
void marker(int num)
{
	srand(num);
	int count = 0;
	std::vector<int> mass;
	while (true) {
		std::unique_lock<std::mutex> ul(signal);
		if (number == num) {
			checkArr[num] = false;
			for (int i = 0; i < mass.size(); i++) {
				arr[mass[i]] = 0;
			}

			con.notify_all();
			ul.unlock();
			return;
		}
		int temp = rand() % n;
		if (arr[temp] == 0) {
			Sleep(5);
			arr[temp] = num + 1;
			if (!check(mass, temp)) {
				mass.push_back(temp);
			}
			Sleep(5);
		}
		else {
			checkArr[num] = false;
			cout << num << " " << mass.size() << " " << temp << endl;
			con.notify_all();
			con.wait(ul, [=]() {return checkArr[num]; });
		}
		ul.unlock();
	}
}
int main() {
	cin >> n;
	arr = new int[n];
	for (int i = 0; i < n; i++) {
		arr[i] = 0;
	}
	cin >> markerCount;
	std::vector<std::thread> hThreads;
	checkArr = new bool[markerCount];
	for (int i = 0; i < markerCount; i++) {
		hThreads.emplace_back(&marker, i);
		checkArr[i] = true;
	}
	for (int i = 0; i < markerCount; i++) {
		hThreads[i].detach();
	}
	std::vector<int> endOftheThreads;
	int temp = markerCount;
	while (markerCount > 0) {
		std::unique_lock<std::mutex> ul(signal);
		con.wait(ul, [=]() {return StopThreads(); });
		for (int i = 0; i < n; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
		int index;
		cout << "Enter the number of the thread to be completed: ";
		cin >> index;
		index--;
		if (index >= temp || index < 0) {
			cout << "Input error." << endl;
			break;
		}
		else {
			endOftheThreads.push_back(index);
			checkArr[n] = true;
			number = n;
			con.notify_all();
			con.wait(ul, [=]() {return StopThreads(); });
			for (int i = 0; i < markerCount; i++) {
				if (!check(endOftheThreads, i)) {
					checkArr[i] = true;
				}
			}
			con.notify_all();
			ul.unlock();
			markerCount--;
		}
	}
	return 0;
}