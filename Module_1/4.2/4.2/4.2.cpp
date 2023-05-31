//Имеется лог - файл, в котором хранятся пары для N пользователей(Идентификатор пользователя, посещаемость сайта).
//Напишите программу, которая выбирает K пользователей, которые чаще других заходили на сайт, и выводит их в порядке возрастания посещаемости.
//Количество заходов и идентификаторы пользователей не повторяются.

#include <iostream>
#include <cassert>

template<class T>
class IsLessCompareDefault;

class IsLessCompareByCount;

struct User {
    size_t id;

    size_t count;
};

template<class T, class Compare = IsLessCompareDefault<T>>
class Heap {
public:
    Heap(Compare cmp = IsLessCompareDefault<T>()) : _cmp(cmp), _size(0), _bufferSize(1) { _buffer = new T[_bufferSize]; };

    Heap(const T* arr, int size, Compare cmp = IsLessCompareDefault<T>()) : _size(size), _cmp(cmp), _bufferSize(size)
    {
        assert(arr != nullptr);
        _buffer = new T[size];
        for (int i = 0; i < size; ++i)
            _buffer[i] = arr[i];
        BuildHeap();
    };

    ~Heap() { delete[] _buffer; };

    void Insert(const T& element)
    {
        if (_size + 1 > _bufferSize)
            Resize(_size, _bufferSize * 2);
        _buffer[_size] = element;
        _size++;
        SiftUp(_size - 1);
    };

    T ExtractMin()
    {
        assert(_buffer != nullptr);
        T result = _buffer[0];
        _buffer[0] = _buffer[_size - 1];
        Resize(_size - 1, _bufferSize);
        if (!IsEmpty()) {
            SiftDown(0);
        }
        return result;
    };

    T& Peek() const { return _buffer[0]; };

    bool IsEmpty() { return _size == 0; };

private:
    Compare _cmp;

    T* _buffer;

    size_t _bufferSize;

    size_t _size;

    void BuildHeap()
    {
        for (int i = _size / 2 - 1; i >= 0; --i) {
            SiftDown(i);
        }
    };

    void SiftDown(int i)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < _size && _cmp(_buffer[left], _buffer[i]))
            smallest = left;
        if (right < _size && _cmp(_buffer[right], _buffer[smallest]))
            smallest = right;
        if (smallest != i) {
            std::swap(_buffer[i], _buffer[smallest]);
            SiftDown(smallest);
        }
    };

    void SiftUp(int i)
    {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (!_cmp(_buffer[i], _buffer[parent]))
                return;
            std::swap(_buffer[i], _buffer[parent]);
            i = parent;
        }
    };

    void Resize(size_t size, size_t bufferSize)
    {
        T* newBuffer = new T[bufferSize];
        for (size_t i = 0; i < size; ++i)
            newBuffer[i] = _buffer[i];
        delete[] _buffer;
        _buffer = newBuffer;
        _bufferSize = bufferSize;
        _size = size;
    };
};

template<class T>
class IsLessCompareDefault
{
public:
    bool operator()(const T& left, const T& right) { return left < right; };
};

class IsLessCompareByCount
{
public:
    bool operator()(const User& left, const User& right) { return left.count < right.count; };
};

template<class T, class Compare = IsLessCompareDefault<T>>
T* GetTopUsersId(T* users, int n, int k, Compare cmp = IsLessCompareDefault<T>())
{
    assert(users != nullptr);
    assert(n > 0);
    assert(k > 0);
    Heap<T, IsLessCompareByCount> minHeap(users, k, cmp);
    for (int i = k; i < n; ++i) {
        if (cmp(minHeap.Peek(), users[i])) {
            minHeap.ExtractMin();
            minHeap.Insert(users[i]);
        }
    }
    T* result = new T[k];
    int index = 0;
    while (!minHeap.IsEmpty())
        result[index++] =  minHeap.ExtractMin();
    return result;
}

int main()
{
    int n, k;
    std::cin >> n >> k;
    User* users = new User[n];
    for (int i = 0; i < n; ++i) {
        User tmp;
        std::cin >> tmp.id >> tmp.count;
        users[i] = tmp;
    }
    User* result = GetTopUsersId(users, n, k, IsLessCompareByCount());
    delete[] users;
    for (int i = 0; i < k; ++i)
        std::cout << result[i].id << " ";
    delete[] result;
}