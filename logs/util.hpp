#pragma once
#ifndef __M_UTIL_H
#define __M_UTIL_H
#include <ctime>
#include <sys/stat.h>
#include <string>
#include <iostream>
namespace log
{
    class Date
    {
    public:
        static size_t now()
        {
            return (size_t)time(nullptr);
        }
    };

    class File
    {
    public:
        static bool exists(const std::string &pathname)
        {
            struct stat st;
            if (stat(pathname.c_str(), &st) < 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        static std::string path(const std::string &pathname)
        {
            size_t pos = pathname.find_last_of("/\\");
            if(std::string::npos == pos)
            {
                return ".";
            }
            return pathname.substr(0, pos + 1);
        }

        static void CreateDirectory(const std::string& pathname)
        {
            size_t pos = 0, idx = 0;
            while(idx < pathname.size())
            {
                pos = pathname.find_first_of("/\\",idx);
                // std::cout << pos << std::endl;
                if(std::string::npos == pos)
                {
                    if(!exists(pathname.c_str()))
                    {
                        mkdir(pathname.c_str(), 0777);
                    }
                    // return ;
                }
                std::string parent_dir = pathname.substr(0, pos + 1);
                if(exists(parent_dir))
                {
                    idx = pos + 1;
                    continue;
                }
                // std::cout << parent_dir << std::endl;
                mkdir(parent_dir.c_str(), 0777);
                idx = pos + 1;
            }
        }
    };

};

#endif // !__M_UTIL_H