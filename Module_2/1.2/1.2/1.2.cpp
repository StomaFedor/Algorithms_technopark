//Реализуйте структуру данных типа “множество строк” на основе динамической хеш - таблицы с открытой адресацией.
//Хранимые строки непустые и состоят из строчных латинских букв.
//Хеш - функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
//Начальный размер таблицы должен быть равным 8 - ми.Перехеширование выполняйте при добавлении элементов в случае, 
//когда коэффициент заполнения таблицы достигает 3 / 4.
//Структура данных должна поддерживать операции добавления строки в множество,
//удаления строки из множества и проверки принадлежности данной строки множеству.
//1_2.Для разрешения коллизий используйте двойное хеширование.

#include <iostream>
#include <vector>
#include <string>
#include <exception>

using std::vector;
using std::string;

struct StringHasher
{
    size_t operator()(const string& key) const
    {
        size_t hash = 0;
        for (size_t i = 0; i < key.size(); ++i)
            hash = hash * 137 + key[i];
        return hash;
    }
};

template<class T, class H>
class HashTable
{
public:
    HashTable(int initialSize, const H& hasher) : _hasher(hasher), _table(initialSize), _keysCount(0) { }

    HashTable(const H& hasher) : _table(8), _hasher(hasher), _keysCount(0) { }

    ~HashTable() { }

    bool Has(const T& key) const;

    bool Add(const T& key);
    
    bool Delete(const T& key);

private:
    H _hasher;

    enum State { Empty, Key, Deleted };

    struct HashTableStruct
    {
        HashTableStruct(const T& key, size_t hash, State status) : Key(key), Hash(hash), Status(status) { }

        HashTableStruct() : Status(State::Empty), Hash(0) { }

        T Key;

        size_t Hash;

        State Status;
    };

    vector<HashTableStruct> _table;

    size_t _keysCount;

    size_t _deletedCount;

    void growTable();
};

template<class T, class H>
bool HashTable<T, H>::Has(const T& key) const
{
    size_t absHash = _hasher(key);
    for (size_t i = 0; i < _table.size(); ++i) {
        size_t hash = (absHash + i * (absHash * 2 + 1)) % _table.size();
        if (_table[hash].Status == State::Key && _table[hash].Key == key)
            return true;
        else if (_table[hash].Status == State::Empty)
            return false;
    }
    return false;
}

template<class T, class H>
bool HashTable<T, H>::Add(const T& key)
{
    if (_keysCount >= _table.size() * 0.75)
        growTable();
    size_t absHash = _hasher(key);
    int deleted = -1;
    for (size_t i = 0; i < _table.size(); ++i) {
        size_t hash = (absHash + i * (absHash * 2 + 1)) % _table.size();
        if (_table[hash].Status == State::Deleted && deleted == -1)
            deleted = hash;
        else if (_table[hash].Status == State::Empty && deleted == -1) {
            HashTableStruct tmp = HashTableStruct(key, absHash, State::Key);
            _table[hash] = std::move(tmp);
            _keysCount++;
            return true;
        }
        else if (_table[hash].Status == State::Empty && deleted != -1) {
            HashTableStruct tmp = HashTableStruct(key, absHash, State::Key);
            _table[deleted] = tmp;
            _keysCount++;
            return true;
        }
        else if (_table[hash].Status == State::Key && _table[hash].Key == key)
            return false;
    }
    throw std::overflow_error("Overflow");
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T& key)
{
    size_t absHash = _hasher(key);
    for (size_t i = 0; i < _table.size(); ++i) {
        size_t hash = (absHash + i * (absHash * 2 + 1)) % _table.size();
        if (_table[hash].Status == State::Key && _table[hash].Key == key) {
            _table[hash].Status = State::Deleted;
            _deletedCount++;
            return true;
        }
        else if (_table[hash].Status == State::Empty)
            return false;
    }
    return false;
}

template<class T, class H>
void HashTable<T, H>::growTable()
{
    size_t newSize = _deletedCount >= _table.size() * 0.5 ? _table.size() : _table.size() * 2;
    size_t newKeysCount = 0;
    vector<HashTableStruct> newTable(newSize);
    for (size_t i = 0; i < _table.size(); ++i) {
        HashTableStruct tmp = _table[i];
        size_t absHash = tmp.Hash;
        size_t count = 0;
        while (tmp.Status == State::Key) {
            size_t hash = (absHash + count * (absHash * 2 + 1)) % newSize;
            if (newTable[hash].Status == State::Empty) {
                newTable[hash] = tmp;
                newKeysCount++;
                break;
            }
            count++;
        }
    }
    _deletedCount = 0;
    _keysCount = newKeysCount;
    _table = std::move(newTable);
}

int main()
{
    StringHasher h;
    HashTable<string, StringHasher> table(8, h);
    char operation = 0;
    string word;
    while (std::cin >> operation >> word) {
        switch (operation) {
        case '+':
            std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
            break;
        case '?':
            std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }
    return 0;
}
