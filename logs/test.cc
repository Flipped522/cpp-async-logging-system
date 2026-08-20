#include "util.hpp"
#include <iostream>

int main()
{
    // std::cout << Util::Date::now() << std::endl;
    std::string pathname = "./a/b/a.txt";
    // std::cout << Util::File::path(pathname) << std::endl;
    Util::File::CreateDirectory(Util::File::path(pathname));

    return 0;
}