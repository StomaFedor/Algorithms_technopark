// Рк_1_1.2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cassert>

template<class T>
class IsLessCompareDefault
{
public:
    bool operator()(const T& left, const T& right) { return left < right; };
};

template<class T, class Compare = IsLessCompareDefault<T>>
int BinarySearch(const T* arr, const int len, T element, Compare cmp = IsLessCompareDefault<T>())
{
    assert(arr != nullptr);
    assert(len > 0);
    int first = 0;
    int last = len;
    while (first < last) {
        int mid = (first + last) / 2;
        if (cmp(arr[mid], element))
            first = mid + 1;
        else
            last = mid;
    }
    return first == len ? len : first;
}

int main()
{
    int n, k;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i)
        std::cin >> arr[i];
    std::cin >> k;
    std::cout << BinarySearch(arr, n, k);
    delete[] arr;
}