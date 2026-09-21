#pragma once
#include "util.hpp"
#include "message.hpp"
#include "format.hpp"
#include "sink.hpp"
#include <iostream>

int main()
{
    log::LogMsg msg(log::LogLevel::value::INFO, 53, "main.c", "root", "格式化功能测试...");
    log::Formatter fmt;
    std::string str = fmt.format(msg);
    log::LogSink::ptr stdout_lsp = log::SinkFactory::create<log::StdoutSink>();
    log::LogSink::ptr file_lsp = log::SinkFactory::create<log::FileSink>("./logfile/test.log");
    log::LogSink::ptr roll_lsp = log::SinkFactory::create<log::RollBySizeSink>("./logfile/roll-", 1024 * 1024);
    stdout_lsp->log(str.c_str(), str.size());
    file_lsp->log(str.c_str(), str.size());
    size_t cursize = 0;
    size_t count = 0;
    while(cursize < 1024 * 1024 * 10)
    {
        std::string tmp = str + std::to_string(count++);
        roll_lsp->log(tmp.c_str(), tmp.size());
        cursize += str.size();
    }


    return 0;
}