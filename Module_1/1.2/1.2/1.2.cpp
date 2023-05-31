//Вернуть значение бита в числе N по его номеру K.
//Позиции битов нумеруются с 0.
//Необходимо использование битовых операций.
//Использование арифметических операций запрещено.

#include <iostream>
#include <cassert>

int GetBitValue(unsigned int value, unsigned int bitNumber);

int main()
{
    unsigned int n, k;
    std::cin >> n;
    std::cin >> k;

    int result = GetBitValue(n, k);

    std::cout << result;
}

int GetBitValue(unsigned int value, unsigned int bitNumber)
{
    assert(value >= 0);
    assert(0 <= bitNumber < 32);
    int result = value >> bitNumber & 1;
    std::cout << (value >> bitNumber);
    return result;
}
