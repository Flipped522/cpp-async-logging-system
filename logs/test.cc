#pragma once
#include "util.hpp"
#include "message.hpp"
#include "format.hpp"
#include "sink.hpp"
#include "logger.hpp"
#include "buffer.hpp"
#include <iostream>
#include <unistd.h>

/*扩展一个以时间作为日志文件滚动切换的类型的日志落地模块
    1. 以时间进行文件滚动，实际上是以时间段进行滚动
        实现思想： 以当前系统时间，取模时间段大小，可以得到当前时间段是第几个时间段
        time(nullptr) % gap;
*/

enum class TimeGap
{
    GAP_SECOND,
    GAP_MINUTE,
    GAP_HOUR,
    GAP_DAY
};

class RollByTimeSink : public log::LogSink
{
public:
    // 构造时传入文件名，并打开文件，将操作句柄管理起来
    RollByTimeSink(const std::string &basename, TimeGap gap_type) : _basename(basename)
    {
        switch (gap_type)
        {
        case TimeGap::GAP_SECOND:
            _gap_size = 1;
            break;
        case TimeGap::GAP_MINUTE:
            _gap_size = 60;
            break;
        case TimeGap::GAP_HOUR:
            _gap_size = 3600;
            break;
        case TimeGap::GAP_DAY:
            _gap_size = 3600 * 24;
            break;
        }
        _cur_gap = _gap_size == 1 ? log::util::Date::now() : log::util::Date::now() % _gap_size;
        std::string filename = createNewFile();
        log::util::File::CreateDirectory(log::util::File::path(filename));
        _ofs.open(filename, std::ios::binary | std::ios::app);
        assert(_ofs.is_open());
    }
    // 将日志消息 写入到标准输出, 判断当前时间是否是当前文件的时间段，不是则切换文件
    void log(const char *data, size_t len)
    {
        time_t cur = log::util::Date::now();
        if ((cur % _gap_size) != _cur_gap)
        {
            _ofs.close();
            std::string filename = createNewFile();
            _ofs.open(filename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        _ofs.write(data, len);
        assert(_ofs.good());
    }

private:
    std::string createNewFile()
    {
        time_t t = log::util::Date::now();
        struct tm lt;
        localtime_r(&t, &lt);
        std::stringstream filename;
        filename << _basename;
        filename << lt.tm_year + 1900;
        filename << lt.tm_mon + 1;
        filename << lt.tm_mday;
        filename << lt.tm_hour;
        filename << lt.tm_min;
        filename << lt.tm_sec;
        filename << ".log";

        return filename.str();
    }

private:
    std::string _basename;
    std::ofstream _ofs;
    size_t _cur_gap;  // 当前是第几个时间段
    size_t _gap_size; // 时间段大小
};

int main()
{
    // log::LogMsg msg(log::LogLevel::value::INFO, 53, "main.c", "root", "格式化功能测试...");
    // log::Formatter fmt;
    // std::string str = fmt.format(msg);
    // // log::LogSink::ptr stdout_lsp = log::SinkFactory::create<log::StdoutSink>();
    // // log::LogSink::ptr file_lsp = log::SinkFactory::create<log::FileSink>("./logfile/test.log");
    // // log::LogSink::ptr roll_lsp = log::SinkFactory::create<log::RollBySizeSink>("./logfile/roll-", 1024 * 1024);
    // log::LogSink::ptr time_lsp = log::SinkFactory::create<RollByTimeSink>("./logfile/roll-",TimeGap::GAP_SECOND);

    // // stdout_lsp->log(str.c_str(), str.size());
    // // file_lsp->log(str.c_str(), str.size());
    // // size_t cursize = 0;
    // // size_t count = 0;
    // // while (cursize < 1024 * 1024 * 10)
    // // {
    // //     std::string tmp = str + std::to_string(count++);
    // //     roll_lsp->log(tmp.c_str(), tmp.size());
    // //     cursize += str.size();
    // // }
    // time_t old = log::util::Date::now();
    // while(log::util::Date::now() < old + 5)
    // {
    //     time_lsp->log(str.c_str(), str.size());
    //     usleep(1000);
    // }

    // std::string logger_name = "sync_logger";
    // log::LogLevel::value limit = log::LogLevel::value::WARN;
    // log::Formatter::ptr fmt(new log::Formatter("[%d{%H:%M:%S}][%c][%f:%l][%p]%T%m%n"));

    // log::LogSink::ptr stdout_lsp = log::SinkFactory::create<log::StdoutSink>();
    // log::LogSink::ptr file_lsp = log::SinkFactory::create<log::FileSink>("./logfile/test.log");
    // log::LogSink::ptr roll_lsp = log::SinkFactory::create<log::RollBySizeSink>("./logfile/roll-", 1024 * 1024);
    // std::vector<log::LogSink::ptr> sinks = {stdout_lsp, file_lsp, roll_lsp};
    // log::Logger::ptr logger(new log::SyncLogger(logger_name, limit, fmt, sinks));

    // std::unique_ptr<log::LoggerBuilder> builder(new log::LocalLoggerBuilder());
    // builder->buildLoggerName("sync_logger");
    // builder->buildLoggerLevel(log::LogLevel::value::WARN);
    // builder->buildFormatter("%m%n");
    // builder->buildLoggerType(log::LoggerType::LOGGER_SYNC);
    // builder->buildSink<log::FileSink>("./logfile/test.log");
    // builder->buildSink<log::StdoutSink>();
    // log::Logger::ptr logger = builder->build();

    // logger->debug(__FILE__, __LINE__, "%s", "测试日志");
    // logger->info(__FILE__, __LINE__, "%s", "测试日志");
    // logger->warn(__FILE__, __LINE__, "%s", "测试日志");
    // logger->error(__FILE__, __LINE__, "%s", "测试日志");
    // logger->fatal(__FILE__, __LINE__, "%s", "测试日志");
    // size_t cursize = 0;
    // size_t count = 0;
    // while (cursize < 1024 * 1024 * 10)
    // {
    //     logger->fatal(__FILE__, __LINE__, "测试日志-%d", count++);
    //     cursize += 20;
    // }

    std::ifstream ifs("./logfile/test.log", std::ios::binary);
    if (ifs.is_open() == false)
    {
        std::cout << "open failed\n";
        return -1;
    }

    ifs.seekg(0, std::ios::end); // 读写位置跳转到文件末尾
    size_t fsize = ifs.tellg();  // 获取当前位置相对于起始位置的偏移量
    ifs.seekg(0, std::ios::beg); // 重新跳转到起始位置
    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);
    if(ifs.good() == false)
    {
        std::cout << "read error\n";
        return -1;
    }

    std::cout << fsize << std::endl;

    ifs.close();
    log::Buffer buffer;
    for(int i = 0;i < body.size(); ++i)
    {
        buffer.push(&body[i], 1);
    }
    std::cout << buffer.readAbleSize() << std::endl;

    std::ofstream ofs("./logfile/tmp.log", std::ios::binary);
    ofs.write(buffer.begin(), buffer.readAbleSize());
    for(int i = 0;i < buffer.readAbleSize(); ++i)
    {
        ofs.write(buffer.begin(), 1);
        if(ofs.good() == false)
        {
            std::cout << "write error!\n";
            return -1;
        }
        buffer.moveReader(1);
    }

    ofs.close();

    return 0;
}