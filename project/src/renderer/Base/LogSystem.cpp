#include "LogSystem.h"
#include "iostream"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

spdlog::logger* LogSystem::Logger = nullptr;
LogSystem::LogSystem()
{
}

LogSystem::~LogSystem()
{
}

void LogSystem::init()
{

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[JNENGINE_LOG_INFO] [%^%l%$] %v");

    auto basic_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/JNLog.txt", true);
    basic_file_sink->set_level(spdlog::level::trace);

    auto max_size = 1048576 * 5;
    auto max_files = 3;
    auto rotating_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/RotJNLog.txt", max_size, max_files);
    rotating_file_sink->set_level(spdlog::level::trace);

    Logger = new spdlog::logger("JNLog", { console_sink, basic_file_sink, rotating_file_sink });
    Logger->set_level(spdlog::level::trace);
    Logger->flush_on(spdlog::level::trace);
    //Logger.warn("this should appear in both console and file");
    //Logger.info("this message should not appear in the console, only in the file");
}

spdlog::logger* LogSystem::getLogger()
{
    if (Logger == nullptr)
    {
        std::cout << "LogSystem 未初始化或初始化失败，请检查！" << std::endl;
    }
    return Logger;
}
