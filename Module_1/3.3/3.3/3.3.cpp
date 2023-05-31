//Реализовать очередь с помощью двух стеков.Использовать стек, реализованный с помощью динамического буфера.
//Обрабатывать команды push back и pop front.

#include <iostream>
#include <cassert>

template<class T>
class Stack {
public:
    explicit Stack(int bufferSize) : _bufferSize(bufferSize), _top(-1)
    {
        _buffer = new T[_bufferSize];
    };

    ~Stack() { delete[] _buffer; };

    void Push(const T& a)
    {
        if (_top + 1 >= _bufferSize)
        {
            T* newBuffer = new T[_bufferSize * 2];
            for (int i = 0; i < _top + 1; ++i)
                newBuffer[i] = _buffer[i];
            delete[] _buffer;
            _buffer = newBuffer;
            _bufferSize *= 2;
        }
        _buffer[++_top] = a;
    };

    T Pop()
    {
        assert(_top != -1);
        return _buffer[_top--];
    };

    bool IsEmpty() const { return _top == -1; };

private:
    T* _buffer;

    int _bufferSize;

    int _top;
};

template<class T>
class Queue
{
public:
    Queue() : _popStack(1), _pushStack(1) {};

    ~Queue() { };

    void Enqueue(const T& data)
    {
        _pushStack.Push(data);
    };

    T Dequeue()
    {
        assert(!IsEmpty());
        if (_popStack.IsEmpty())
            while (!_pushStack.IsEmpty())
                _popStack.Push(_pushStack.Pop());
        return _popStack.Pop();
    };

    bool IsEmpty() const { return (_pushStack.IsEmpty() && _popStack.IsEmpty()); };

private:
    Stack<T> _pushStack;

    Stack<T> _popStack;
};

int main()
{
    int n;
    std::cin >> n;
    Queue<int> queue;
    bool result = true;
    for (int i = 0; i < n; ++i)
    {
        int command, data;
        std::cin >> command >> data;
        switch (command)
        {
        case 2:
            if (queue.IsEmpty())
                result = result && data == -1;
            else
                result = result && queue.Dequeue() == data;
            break;
        case 3:
            queue.Enqueue(data);
            break;
        default:
            assert(false);
        }
    }
    std::cout << (result ? "YES" : "NO");
}