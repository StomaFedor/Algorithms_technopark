//Дан массив неотрицательных целых 64 - битных чисел.Количество чисел не больше 1000000. 
//Отсортировать массив методом поразрядной сортировки LSD по байтам.

#include <iostream>
#include <cassert>
#include <cstring>

template<class T>
class GetKeyDefault
{
public:
	T operator()(const T& key) { return key; };
};

template<class T, class TGetKey = GetKeyDefault<T>>
void CountingSort(T* arr, int n, TGetKey getKey = GetKeyDefault<T>()) {
	assert(arr != nullptr);
	assert(n > 0);
	long long max = getKey(arr[0]);
	for (int i = 1; i < n; ++i) {
		max = std::max(max, getKey(arr[i]));
	}
	int* countArr = new int[max + 1];
	std::memset(countArr, 0, (max + 1) * sizeof(int));

	for (int i = 0; i < n; ++i)
		++countArr[getKey(arr[i])];

	for (int i = 1; i < max + 1; ++i) {
		countArr[i] += countArr[i - 1];
	}

	T* newArr = new T[n];
	for (int i = n - 1; i >= 0; --i) {
		newArr[--countArr[getKey(arr[i])]] = arr[i];
	}
	delete[] countArr;
	for (int i = 0; i < n; ++i) {
		arr[i] = newArr[i];
	}
	delete[] newArr;
}

void LsdByBytes(long long* arr, int n)
{
	assert(arr != nullptr);
	assert(n > 0);
	//в целом можно сделать цикл, но для этого надо завести глобальную переменную, чтобы ее было видно в лямбда выражении. Не знаю, насколько это лучше
	CountingSort(arr, n, [](long long key) { return key & 255; });
	CountingSort(arr, n, [](long long key) { return (key >> 8 * 1) & 255; });
	CountingSort(arr, n, [](long long key) { return (key >> 8 * 2) & 255; });
	CountingSort(arr, n, [](long long key) { return (key >> 8 * 3) & 255; });
	CountingSort(arr, n, [](long long key) { return (key >> 8 * 4) & 255; });
	CountingSort(arr, n, [](long long key) { return (key >> 8 * 5) & 255; });
	CountingSort(arr, n, [](long long key) { return (key >> 8 * 6) & 255; });
	CountingSort(arr, n, [](long long key) { return (key >> 8 * 7) & 255; });

}

int main()
{
    int n;
    std::cin >> n;
    long long* arr = new long long[n];
    for (int i = 0; i < n; ++i)
        std::cin >> arr[i];
	LsdByBytes(arr, n);
	for (int i = 0; i < n; ++i)
		std::cout << arr[i] << " ";
}