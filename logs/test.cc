#pragma once
#include "util.hpp"
#include "message.hpp"
#include "format.hpp"
#include <iostream>

int main()
{
    log::LogMsg msg(log::LogLevel::value::INFO, 53, "main.c", "root", "格式化功能测试...");
    log::Formatter fmt("abc%%abc[%d{%H:%M:%S}] %m%n%g");
    std::string str = fmt.format(msg);
    std::cout <<  str << std::endl;

    return 0;
}