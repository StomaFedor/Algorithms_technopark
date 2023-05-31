//В супермаркете решили оптимизировать показ рекламы.Известно расписание прихода и ухода покупателей(два целых числа).
//Каждому покупателю необходимо показать минимум 2 рекламы.
//Рекламу можно транслировать только в целочисленные моменты времени.
//Покупатель может видеть рекламу от момента прихода до момента ухода из магазина
//.В каждый момент времени может показываться только одна реклама.
//Считается, что реклама показывается мгновенно.
//Если реклама показывается в момент ухода или прихода, то считается, что посетитель успел её посмотреть.
//Требуется определить минимальное число показов рекламы.

#include <iostream>
#include <cassert>
#include <cstring>

struct Customer
{
    size_t startTime;

    size_t stopTime;
};

class IsLessCompareByStopTime
{
public:
    bool operator()(const Customer& left, const Customer& right) 
    {
        if (left.stopTime == right.stopTime)
            return left.startTime < right.startTime;
        else
            return left.stopTime < right.stopTime;
    };
};

template<class T>
class IsLessCompareDefault
{
public:
    bool operator()(const T& left, const T& right) { return left < right; };
};

template<class T, class Compare = IsLessCompareDefault<T>>
void Merge(T* firstArr, int firstLen, T* secondArr, int secondLen, T* result, Compare cmp = IsLessCompareDefault<T>())
{
    int firstInd = 0, secondInd = 0, resultInd = 0;
    while (firstInd < firstLen && secondInd < secondLen)
    {
        if (!cmp(secondArr[secondInd], firstArr[firstInd]))
            result[resultInd++] = firstArr[firstInd++];
        else
            result[resultInd++] = secondArr[secondInd++];
    }
    while (firstInd < firstLen || secondInd < secondLen)
    {
        if (firstInd < firstLen)
            result[resultInd++] = firstArr[firstInd++];
        else
            result[resultInd++] = secondArr[secondInd++];
    }
}

template<class T, class Compare = IsLessCompareDefault<T>>
void MergeSort(T* arr, int len, Compare cmp = IsLessCompareDefault<T>())
{
    assert(arr != nullptr);
    assert(len > 0);
    if (len <= 1)
        return;
    int firstLen = len / 2;
    int secondLen = len - firstLen;
    MergeSort(arr, firstLen, cmp);
    MergeSort(arr + firstLen, secondLen, cmp);
    T* tmpArr = new T[len];
    Merge(arr, firstLen, arr + firstLen, secondLen, tmpArr, cmp);
    std::memcpy(arr, tmpArr, sizeof(T) * len);
    delete[] tmpArr;
}

int GetMinAdvertisementCount(Customer* customers, int n)
{
    assert(customers != nullptr);
    assert(n > 0);
    MergeSort(customers, n, IsLessCompareByStopTime());
    int count = 2, firstTime = customers[0].stopTime - 1, secondTime = customers[0].stopTime;
    for (int i = 1; i < n; ++i)
    {
        if (secondTime < customers[i].startTime) {
            count += 2;
            firstTime = customers[i].stopTime - 1;
            secondTime = customers[i].stopTime;
        }
        else if (secondTime == customers[i].startTime) {
            count++;
            firstTime = secondTime;
            secondTime = customers[i].stopTime;
        }
        else if (firstTime < customers[i].startTime && secondTime < customers[i].stopTime) {
            count++;
            firstTime = secondTime;
            secondTime = customers[i].stopTime;
        }
        else if (firstTime < customers[i].startTime && secondTime > customers[i].startTime) {
            count++;
            firstTime = customers[i].stopTime;
        }
    }
    return count;
}

int main()
{
    int n;
    std::cin >> n;
    Customer* customers = new Customer[n];
    for (int i = 0; i < n; ++i) {
        Customer tmp;
        std::cin >> tmp.startTime >> tmp.stopTime;
        customers[i] = tmp;
    }
    int result = GetMinAdvertisementCount(customers, n);
    delete[] customers;
    std::cout << result;
}
