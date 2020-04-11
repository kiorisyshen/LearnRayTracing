#pragma once
#include <unordered_map>
#include "spdlog/spdlog.h"

namespace LearnRT {

class Logger {
   public:
    static void AddBasicFileSink(std::string filefullpath, std::string loggerName = "Main", spdlog::level::level_enum logLvl = spdlog::level::debug);
    static void AddRotateFileSink(std::string filefullpath, std::size_t maxFileSizeInByte = 10 * 1024 * 1024, std::size_t maxNumOfFile = 5,
                                  std::string loggerName = "Main", spdlog::level::level_enum logLvl = spdlog::level::trace);
    static void AddCoutSink(std::string loggerName = "Main", spdlog::level::level_enum logLvl = spdlog::level::debug);
    static void AddCerrSink(std::string loggerName = "Main", spdlog::level::level_enum logLvl = spdlog::level::warn);
    static void AddNullSink(std::string loggerName = "Main");

    static spdlog::logger &GetLogger(std::string name = "Main");

    static void ClearAll();

   private:
    Logger() {
    }

    static void AddSinkToMap(std::string sinkName, std::string loggerName);
    static void AssertLogger(std::string loggerName);
};

}  // namespace LearnRT