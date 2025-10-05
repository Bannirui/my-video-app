//
// Created by dingrui on 10/5/25.
//

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>

#define MY_LOG_INFO(...) MyLog::GetLogger()->info(__VA_ARGS__)
#define MY_LOG_ERROR(...) MyLog::GetLogger()->error(__VA_ARGS__)

class MyLog {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger> GetLogger() {
        if (!logger) {
            // 兜底 保证不会崩溃
            Init();
        }
        return logger;
    }

private:
    static std::shared_ptr<spdlog::logger> logger;
};
