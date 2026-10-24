#pragma once
#include "../logs/log.h"
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
    std::unique_ptr<log::LoggerBuilder> builder(new log::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildLoggerLevel(log::LogLevel::value::WARN);
    builder->buildFormatter("[%c][%f:%l]%m%n");
    builder->buildLoggerType(log::LoggerType::LOGGER_ASYNC);
    builder->buildEnableUnSafeAsync();
    builder->buildSink<RollByTimeSink>("./logfile/roll-async-by-time", TimeGap::GAP_SECOND);
    log::Logger::ptr logger = builder->build();

    size_t cur = log::util::Date::now();

    while(log::util::Date::now() < cur + 5)
    {
        logger->fatal("这是一条测试日志");
        usleep(1000);
    }

    return 0;
}