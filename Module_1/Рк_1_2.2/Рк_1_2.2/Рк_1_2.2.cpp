// Рк_1_2.2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cassert>
#include <string>

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

    T Top()
    {
        assert(_top != -1);
        return _buffer[_top];
    }

    bool IsEmpty() const { return _top == -1; };

private:
    T* _buffer;

    int _bufferSize;

    int _top;
};

bool ConvertStrWithStack(const std::string& str, const std::string& newStr)
{
    assert(str.size() != 0);
    assert(str.size() != 0);
    Stack<char> stack(str.size());
    std::string result;
    int i = 0, j = 0;
    while (i < str.size() && j < newStr.size()) {
        if (str[i] == newStr[j]) {
            result += str[i++];
            ++j;
        }
        else if (!stack.IsEmpty() && newStr[j] == stack.Top()) {
            ++j;
            result += stack.Pop();
        }
        else {
            stack.Push(str[i++]);

        }
    }
    while (!stack.IsEmpty())
        result += stack.Pop();
    return result == newStr;
}

int main()
{
    std::string str, newStr;
    std::cin >> str >> newStr;
    std::cout << (ConvertStrWithStack(str, newStr) ? "YES" : "NO");
}
