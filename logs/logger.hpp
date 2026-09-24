/*
    1. 抽象日志器基类
    2.  派生出不同的子类（同步日志器类 & 异步日志器类）
*/
#pragma once
#include "util.hpp"
#include "level.hpp"
#include "format.hpp"
#include "sink.hpp"
#include <atomic>
#include <mutex>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

namespace log
{
    class Logger
    {
    public:
        using ptr = std::shared_ptr<Logger>;
        Logger(const std::string &logger_name,
               LogLevel::value level,
               Formatter::ptr &formatter,
               std::vector<LogSink::ptr> &sinks) : _logger_name(logger_name),
                                                   _limit_level(level),
                                                   _formatter(formatter),
                                                   _sinks(sinks.begin(), sinks.end())
        {
        }

        void debug(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 1. 通过传入的参数构造日志消息对象，进行日志的格式化，最终落地
            if (LogLevel::value::DEBUG < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串和不定参进行字符串组织，得到的日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (-1 == ret)
            {
                std::cout << "vasprintf failed!\n";
                return;
            }

            va_end(ap);

            serialize(LogLevel::value::DEBUG, file, line, res);

            free(res);
        }

        void info(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 1. 通过传入的参数构造日志消息对象，进行日志的格式化，最终落地
            if (LogLevel::value::INFO < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串和不定参进行字符串组织，得到的日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (-1 == ret)
            {
                std::cout << "vasprintf failed!\n";
                return;
            }

            va_end(ap);

            serialize(LogLevel::value::INFO, file, line, res);

            free(res);
        }

        void warn(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 1. 通过传入的参数构造日志消息对象，进行日志的格式化，最终落地
            if (LogLevel::value::WARN < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串和不定参进行字符串组织，得到的日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (-1 == ret)
            {
                std::cout << "vasprintf failed!\n";
                return;
            }

            va_end(ap);

            serialize(LogLevel::value::WARN, file, line, res);

            free(res);
        }

        void error(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 1. 通过传入的参数构造日志消息对象，进行日志的格式化，最终落地
            if (LogLevel::value::ERROR < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串和不定参进行字符串组织，得到的日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (-1 == ret)
            {
                std::cout << "vasprintf failed!\n";
                return;
            }

            va_end(ap);

            serialize(LogLevel::value::ERROR, file, line, res);

            free(res);
        }

        void fatal(const std::string &file, size_t line, const std::string &fmt, ...)
        {
            // 1. 通过传入的参数构造日志消息对象，进行日志的格式化，最终落地
            if (LogLevel::value::FATAL < _limit_level)
            {
                return;
            }
            // 2. 对fmt格式化字符串和不定参进行字符串组织，得到的日志消息字符串
            va_list ap;
            va_start(ap, fmt);
            char *res;
            int ret = vasprintf(&res, fmt.c_str(), ap);
            if (-1 == ret)
            {
                std::cout << "vasprintf failed!\n";
                return;
            }

            va_end(ap);

            serialize(LogLevel::value::FATAL, file, line, res);

            free(res);
        }

    protected:
        void serialize(LogLevel::value level, const std::string &file, size_t line, char *str)
        {
            // 3. 构造LogMsg对象
            LogMsg msg(level, line, file, _logger_name, str);
            // 4. 通过格式化工具对LogMsg进行格式化，得到格式化后的日志字符串
            std::stringstream ss;
            _formatter->format(ss, msg);
            // 5. 进行日志落地
            log(ss.str().c_str(), ss.str().size());
        }
        virtual void log(const char *data, size_t len) = 0;

    protected:
        std::mutex _mutex;
        std::string _logger_name;
        std::atomic<LogLevel::value> _limit_level;
        Formatter::ptr _formatter;
        std::vector<LogSink::ptr> _sinks;
    };

    class SyncLogger : public Logger
    {
    public:
        SyncLogger(const std::string &logger_name,
                   LogLevel::value level,
                   Formatter::ptr &formatter,
                   std::vector<LogSink::ptr> &sinks) : Logger(logger_name, level, formatter, sinks)
        {
        }

    protected:
        // 同步日志器，日志直接通过落地模块句柄进行日志落地
        void log(const char *data, size_t len)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_sinks.empty())
                return;
            for (auto &sink : _sinks)
            {
                sink->log(data, len);
            }
        }
    };
};