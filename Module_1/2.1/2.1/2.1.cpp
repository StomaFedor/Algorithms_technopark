//Дан отсортированный массив целых чисел A[0..n - 1] и массив целых чисел B[0..m - 1].
//Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, 
//равного или превосходящего B[i]: A[k] >= B[i].Если такого элемента нет, выведите n.
//Время работы поиска k для каждого элемента B[i] : O(log(k)).n, m ≤ 10000.

#include <iostream>
#include <cassert>

int* GetMinIndexes(const int lenA, const int lenB, const int* arrA, const int* arrB);

int main()
{
    int n, m;
    std::cin >> n;
    std::cin >> m;
    int* a = new int[n];
    int* b = new int[m];
    for (int i = 0; i < n; ++i)
        std::cin >> a[i];
    for (int i = 0; i < m; ++i)
        std::cin >> b[i];

    int* result = GetMinIndexes(n, m, a, b);

    delete[] a, b;
    for (int i = 0; i < m; ++i)
        std::cout << result[i] << " ";
    delete[] result;
}

int Min(int x, int y)
{
    return (x < y) ? x : y;
}

int BinarySearch(const int* arr, const int len, int first, int last, int element)
{
    assert(arr != nullptr);
    assert(len > 0);
    assert(first >= 0);
    assert(last > 0);
    while (first < last) {
        int mid = (first + last) / 2;
        if (arr[mid] < element)
            first = mid + 1;
        else
            last = mid;
    }
    return first == len ? len : first;
}

int* GetMinIndexes(const int lenA, const int lenB, const int* arrA, const int* arrB)
{
    assert(lenA > 0);
    assert(lenB > 0);
    assert(arrA != nullptr);
    assert(arrB != nullptr);
    int* result = new int[lenB];
    for (int i = 0; i < lenB; ++i)
    {
        int scope = 1;
        while (scope < lenA && arrA[scope] < arrB[i])
            scope *= 2;
        result[i] = BinarySearch(arrA, lenA, scope / 2, Min(scope, lenA), arrB[i]);
    }
    return result;
}