// Рк_1_3.2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cassert>
#include <random>
#include <ctime>

struct Point {
    int x;
    int y;
    int z;
    int id;
};

int Random(int min, int max)
{
    srand(time(NULL));
    int num = min + rand() % (max - min + 1);
    return num;
}

template<class T>
class IsLessCompareDefault
{
public:
    bool operator()(const T& left, const T& right) { return left < right; };
};

class IsLessCompareByCoord
{
public:
    bool operator()(const Point& left, const Point& right) { return left.x < right.x && left.y < right.y && left.z < right.z; };
};

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
void QuickSort(T* a, int n, Compare cmp = IsLessCompareDefault<T>()) {
    int part = Partition(a, n, cmp);
    if (part > 0) QuickSort(a, part, cmp);
    if (part + 1 < n) QuickSort(a + part + 1, n - (part + 1), cmp);
}

int main()
{
    int n;
    std::cin >> n;
    Point* arr = new Point[n];
    for (int i = 0; i < n; i++)
    {
        Point tmp;
        std::cin >> tmp.x >> tmp.y >> tmp.z;
        tmp.id = i;
        arr[i] = tmp;
    }
    QuickSort(arr, n, IsLessCompareByCoord());
    for (int i = 0; i < n; ++i)
        std::cout << arr[i].id << " ";
}
