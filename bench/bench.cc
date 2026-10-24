#include "../logs/log.h"

void bench(const std::string &loger_name, size_t thread_num, size_t msglen, size_t msg_count)
{
    log::Logger::ptr lp = log::getLogger(loger_name);
    if (lp.get() == nullptr)
        return;
    std::string msg(msglen, '1');
    size_t msg_count_per_thread = msg_count / thread_num;
    std::vector<double> cost_time(thread_num);
    std::vector<std::thread> threads;
    std::cout << "输入线程数量: " << thread_num << std::endl;
    std::cout << "输出日志数量: " << msg_count << std::endl;
    std::cout << "输出日志大小: " << msglen * msg_count / 1024 << "KB" << std::endl;
    for (int i = 0; i < thread_num; i++)
    {
        threads.emplace_back([&, i]()
                             {
            auto start = std::chrono::high_resolution_clock::now();
            for(size_t j = 0; j < msg_count_per_thread; j++) {
                lp->fatal("%s", msg.c_str());
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto cost = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
            cost_time[i] = cost.count();
            auto avg = msg_count_per_thread / cost_time[i];
            std::cout << "线程" << i << "耗时: " << cost.count() << "s" << " 平均：" << (size_t)avg << "/s\n"; });
    }
    for (auto &thr : threads)
    {
        thr.join();
    }
    double max_cost = 0;
    for (auto cost : cost_time)
        max_cost = max_cost < cost ? cost : max_cost;
    std::cout << "\t总消耗时间: " << max_cost << std::endl;
    std::cout << "\t平均每秒输出: " << (size_t)(msg_count / max_cost) << "条日志" << std::endl;
    std::cout << "\t平均每秒输出: " << (size_t)(msglen * msg_count / max_cost / 1024 / 1024) << "MB" << std::endl;
}

void sync_bench()
{
    std::unique_ptr<log::LoggerBuilder> builder(new log::GlobalLoggerBuilder());
    builder->buildLoggerName("sync_logger");
    builder->buildFormatter("%m%n");
    builder->buildLoggerType(log::LoggerType::LOGGER_SYNC);
    builder->buildSink<log::FileSink>("./logfile/sync.log");
    builder->build();
    bench("sync_logger", 3, 100, 100000);
}

void async_bench()
{
    std::unique_ptr<log::LoggerBuilder> builder(new log::GlobalLoggerBuilder());
    builder->buildLoggerName("async_logger");
    builder->buildFormatter("%m%n");
    builder->buildLoggerType(log::LoggerType::LOGGER_ASYNC);
    builder->buildEnableUnSafeAsync();
    builder->buildSink<log::FileSink>("./logfile/async.log");
    builder->build();
    bench("async_logger", 3, 100, 1000000);
}

int main()
{
    async_bench();

    return 0;
}