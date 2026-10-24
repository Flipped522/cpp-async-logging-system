#pragma once
#include "../logs/log.h"
#include <iostream>
#include <unistd.h>

/*扩展一个以时间作为日志文件滚动切换的类型的日志落地模块
    1. 以时间进行文件滚动，实际上是以时间段进行滚动
        实现思想： 以当前系统时间，取模时间段大小，可以得到当前时间段是第几个时间段
        time(nullptr) % gap;
*/

void test_log(const std::string &name)
{
    DEBUG("%s", "测试开始");
    log::Logger::ptr logger = log::LoggerManager::getInstance().getLogger(name);
    logger->debug("%s", "测试日志");
    logger->info("%s", "测试日志");
    logger->warn("%s", "测试日志");
    logger->error("%s", "测试日志");
    logger->fatal("%s", "测试日志");

    INFO("%s", "测试完毕");
}

int main()
{
    std::unique_ptr<log::LoggerBuilder> builder(new log::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildLoggerLevel(log::LogLevel::value::WARN);
    builder->buildFormatter("[%c][%f:%l][%p]%m%n");
    builder->buildLoggerType(log::LoggerType::LOGGER_SYNC);
    builder->buildEnableUnSafeAsync();
    builder->buildSink<log::FileSink>("./logfile/async.log");
    builder->buildSink<log::StdoutSink>();
    builder->buildSink<log::RollBySizeSink>("./logfile/roll-async-by-size", 1024 * 1024);
    builder->build();

    test_log("async_logger");

    return 0;
}