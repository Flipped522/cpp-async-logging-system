#include <iostream>
void xprintf()
{
    std::cout << std::endl;
}


template<class T, class ...Args>
void xprintf(T& v, Args && ...args)
{
    std::cout << v;
    if((sizeof ...(args)) > 0)
    {
        xprintf(std::forward<Args>(args)...);
    }
    else
    {
        xprintf();
    }
}

int main()
{
    xprintf("111", "hhh", "333");

    return 0;
}