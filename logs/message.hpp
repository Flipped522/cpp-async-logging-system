/*定义日志消息类，进行日志中间信息的存储*/
#pragma once
#include "level.hpp"
#include "util.hpp"
#include <iostream>
#include <string>
#include <thread>

namespace log
{
    struct LogMsg
    {
        time_t _ctime;          // 日志产生时间戳
        LogLevel::value _level; // 日志等级
        size_t _line;           // 行号
        std::thread::id _tid;   // 线程id
        std::string _file;      // 源码文件名
        std::string _logger;    // 日志器名称
        std::string _payload;   // 有效消息数据
        LogMsg(LogLevel::value level,
               size_t ctime,
               size_t line,
               const std::string file,
               const std::string logger,
               const std::string msg) : _ctime(Date::now()),
                                        _level(level),
                                        _line(line),
                                        _tid(std::this_thread::get_id()),
                                        _file(file),
                                        _logger(logger),
                                        _payload(_payload)
        {
        }
    };
};