//
// Created by dingrui on 10/5/25.
//

#include "Log.h"


std::shared_ptr<spdlog::logger> MyLog::logger;

void MyLog::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    logger = spdlog::stdout_color_mt("MyVideoApp");
    logger->set_level(spdlog::level::trace);
}