#pragma once

#ifndef __M_FMT_H__
#define __M_FMT_H__
#include <memory>
#include <ctime>
#include <vector>
#include "level.hpp"
#include "message.hpp"

namespace log
{
    // 抽象格式化字符串
    class FormatItem
    {
    public:
        using ptr = std::shared_ptr<FormatItem>;
        virtual void format(std::ostream &out, LogMsg &msg) = 0;
    };

    class MsgFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._payload;
        }
    };
    class LevelFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << LogLevel::toString(msg._level);
        }
    };

    class TimeFormatItem : public FormatItem
    {
    public:
        TimeFormatItem(const std::string &fmt = "%H:%M:%S") : _time_fmt(fmt)
        {
        }
        void format(std::ostream &out, LogMsg &msg) override
        {
            struct tm t;
            localtime_r(&msg._ctime, &t);
            char tmp[32] = {0};
            strftime(tmp, 31, _time_fmt.c_str(), &t);
            out << tmp;
        }

    private:
        std::string _time_fmt; // %H:%M:%S
    };

    class FileFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._file;
        }
    };

    class LinelFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._line;
        }
    };

    class ThreadFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._tid;
        }
    };

    class LoggerFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << msg._logger;
        }
    };

    class TabFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << "\t";
        }
    };

    class NLineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << "\n";
        }
    };

    class OtherFormatItem : public FormatItem
    {
    public:
        OtherFormatItem(const std::string &str) : _str(str)
        {
        }
        void format(std::ostream &out, LogMsg &msg) override
        {
            out << _str;
        }

    private:
        std::string _str;
    };

    class Formatter
    {
        /*
            %d 表示日期，包含子格式{%H:%M:%S}
            %t 表示线程id
            %c 表示日志器名称
            %f 表示源码文件名
            %l 表示源码行号
            %p 表示日志级别
            %T 表示制表符缩进
            %m 表示主体消息
            %n 表示换行
        */
    public:
        Formatter(const std::string &pattern = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n")
            : _pattern(pattern)
        {
        }

        // 对msg进行格式化
        std::string format(LogMsg &msg);
        void format(std::ostream &out, LogMsg &msg);

        // 对格式化规则字符串进行解析
        bool parsePattern();

    private:
        // 根据不同的格式化字符 创建不同的格式化子项对象
        FormatItem::ptr createItem(const std::string &key, const std::string& val);

    private:
        std::string _pattern; // 格式化规则字符串
        std::vector<FormatItem::ptr> _items;
    };
};

#endif // !__M_FMT_H__