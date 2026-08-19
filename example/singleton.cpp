#include <iostream>

// 饿汉方式
// class Singleton
// {
// private:
//     static Singleton _eton;
//     Singleton() : _data(99) {}

//     Singleton(const Singleton &) = delete;
//     ~Singleton() {}

// private:
//     int _data;

// public:
//     static Singleton &getInstance()
//     {
//         return _eton;
//     }
//     int getData() { return _data; }
// };

// Singleton Singleton::_eton;

class Singleton
{
private:
    Singleton() : _data(99) {}

    Singleton(const Singleton &) = delete;
    ~Singleton() {}

private:
    int _data;

public:
    static Singleton &getInstance()
    {
        static Singleton _eton;
        return _eton;
    }
    int getData()
    {
        return _data;
    }
};

int main()
{

    // int data = Singleton::getInstance().getData();
    // std::cout << data << std::endl;

    return 0;
}