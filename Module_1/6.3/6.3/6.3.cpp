//Дано множество целых чисел из[0..10 ^ 9] размера n.
//Используя алгоритм поиска k - ой порядковой статистики, требуется найти следующие параметры множества :
//10 % перцентиль
//медиана
//90 % перцентиль
//Реализуйте стратегию выбора опорного элемента “случайный элемент”.
//Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.



#include <iostream>
#include <cassert>
#include <random>
#include <ctime>

template<class T>
class IsLessCompareDefault
{
public:
    bool operator()(const T& left, const T& right) { return left < right; };
};

int Random(int min, int max)
{
    srand(time(NULL));
    int num = min + rand() % (max - min + 1);
    return num;
}

template<class T, class Compare = IsLessCompareDefault<T>>
int Partition(T* arr, int n, Compare cmp = IsLessCompareDefault<T>()) {
    assert(arr != nullptr);
    assert(n > 0);
    if (n <= 1) {
        return 0;
    }
    int ind = Random(0, n - 1);
    std::swap(arr[ind], arr[n - 1]);
    const T& pivot = arr[n - 1];
    int i = 0, j = 0;
    while (j < n - 1) {
        for (; !cmp(arr[j], pivot) && j < n - 1; ++j) {}
        if (j < n - 1)
            std::swap(arr[i++], arr[j++]);
    }
    std::swap(arr[i], arr[j]);
    return i;
}

template<class T, class Compare = IsLessCompareDefault<T>>
T FindKStat(T* arr, int n, int k, Compare cmp = IsLessCompareDefault<T>()) {
    assert(k < n);
    assert(arr != nullptr);
    assert(n > 0);
    assert(k >= 0);
    while (true) {
        int pivotPos = Partition(arr, n);
        if (pivotPos == k) {
            return arr[k];
        }
        else if (k < pivotPos)
            n = pivotPos;
        else {
            arr += pivotPos + 1;
            n -= pivotPos + 1;
            k -= pivotPos + 1;
        }
    }
}

int main()
{
    int n;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i)
        std::cin >> arr[i];
    std::cout << FindKStat(arr, n, n / 10) << std::endl;
    std::cout << FindKStat(arr, n, n / 2) << std::endl;
    std::cout << FindKStat(arr, n, n * 9 / 10) << std::endl;
    delete[] arr;
}