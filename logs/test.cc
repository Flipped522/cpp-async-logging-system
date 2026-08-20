#pragma once
#include "util.hpp"
#include "message.hpp"
#include <iostream>

int main()
{
    // std::cout << Util::Date::now() << std::endl;
    std::string pathname = "./a/b/a.txt";
    // std::cout << Util::File::path(pathname) << std::endl;
    log::File::CreateDirectory(log::File::path(pathname));

    return 0;
}