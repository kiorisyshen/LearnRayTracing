#include "Logger.hpp"
#include <iostream>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/null_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace LearnRT;

static std::unordered_map<std::string, spdlog::sink_ptr> m_gSinks;
static std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_gLoggers;
static std::unordered_map<std::string, std::vector<spdlog::sink_ptr>> m_gLoggerSinkMap;

void Logger::ClearAll() {
    m_gLoggers.clear();
    m_gLoggerSinkMap.clear();
    m_gSinks.clear();
}
/**
 * @brief Assert if a logger with loggerName is already exist
 *      This is used to avoid wrong code to add sink after logger init
 * 
 * @param loggerName 
 */
void Logger::AssertLogger(std::string loggerName) {
    auto tmp_search_logger = m_gLoggers.find(loggerName);
    if (tmp_search_logger != m_gLoggers.end()) {
        // already exist
        assert(0);
    }
}

void Logger::AddSinkToMap(std::string sinkName, std::string loggerName) {
    AssertLogger(loggerName);

    auto tmp_search_map = m_gLoggerSinkMap.find(loggerName);
    if (tmp_search_map == m_gLoggerSinkMap.end()) {
        // create a map entry for logger
        m_gLoggerSinkMap[loggerName] = {m_gSinks[sinkName]};
    } else {
        m_gLoggerSinkMap[loggerName].push_back(m_gSinks[sinkName]);
    }
}

void Logger::AddBasicFileSink(std::string filefullpath, std::string loggerName, spdlog::level::level_enum logLvl) {
    AssertLogger(loggerName);

    auto tmp_search_sink = m_gSinks.find(filefullpath);
    if (tmp_search_sink == m_gSinks.end()) {
        try {
            // create a basic file sink by its file path
            m_gSinks[filefullpath] = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filefullpath);
            m_gSinks[filefullpath]->set_level(logLvl);
        } catch (const spdlog::spdlog_ex &ex) {
            std::cout << "Basic file sink init failed: " << ex.what() << std::endl;
        }
    }

    AddSinkToMap(filefullpath, loggerName);
}

void Logger::AddRotateFileSink(std::string filefullpath, std::size_t maxFileSizeInByte, std::size_t maxNumOfFile,
                               std::string loggerName, spdlog::level::level_enum logLvl) {
    AssertLogger(loggerName);

    auto tmp_search_sink = m_gSinks.find(filefullpath);
    if (tmp_search_sink == m_gSinks.end()) {
        try {
            // create a basic file sink by its file path
            m_gSinks[filefullpath] = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(filefullpath, maxFileSizeInByte, maxNumOfFile);
            m_gSinks[filefullpath]->set_level(logLvl);
        } catch (const spdlog::spdlog_ex &ex) {
            std::cout << "Rotate file sink init failed: " << ex.what() << std::endl;
        }
    }

    AddSinkToMap(filefullpath, loggerName);
}

void Logger::AddCoutSink(std::string loggerName, spdlog::level::level_enum logLvl) {
    AssertLogger(loggerName);

    std::string sinkName = "_cout";

    auto tmp_search_sink = m_gSinks.find(sinkName);
    if (tmp_search_sink == m_gSinks.end()) {
        // create a basic file sink by its file path
        m_gSinks[sinkName] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        m_gSinks[sinkName]->set_level(logLvl);
    }

    AddSinkToMap(sinkName, loggerName);
}

void Logger::AddCerrSink(std::string loggerName, spdlog::level::level_enum logLvl) {
    AssertLogger(loggerName);

    std::string sinkName = "_cerr";

    auto tmp_search_sink = m_gSinks.find(sinkName);
    if (tmp_search_sink == m_gSinks.end()) {
        // create a basic file sink by its file path
        m_gSinks[sinkName] = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        m_gSinks[sinkName]->set_level(logLvl);
    }

    AddSinkToMap(sinkName, loggerName);
}

void Logger::AddNullSink(std::string loggerName) {
    AssertLogger(loggerName);

    std::string sinkName = "_null";

    auto tmp_search_sink = m_gSinks.find(sinkName);
    if (tmp_search_sink == m_gSinks.end()) {
        // create a basic file sink by its file path
        m_gSinks[sinkName] = std::make_shared<spdlog::sinks::null_sink_mt>();
    }

    AddSinkToMap(sinkName, loggerName);
}

spdlog::logger &Logger::GetLogger(std::string name) {
    auto tmp_search_logger = m_gLoggers.find(name);
    if (tmp_search_logger == m_gLoggers.end()) {
        // search map
        auto tmp_search_map = m_gLoggerSinkMap.find(name);
        if (tmp_search_map == m_gLoggerSinkMap.end()) {
            AddNullSink(name);
        }

        // create logger by map
        m_gLoggers[name] = std::make_shared<spdlog::logger>(name, std::begin(m_gLoggerSinkMap[name]), std::end(m_gLoggerSinkMap[name]));

        // set logger to lowest level of all its sinks
        for (const auto &sinkPtr : m_gLoggerSinkMap[name]) {
            if (m_gLoggers[name]->level() > sinkPtr->level()) {
                m_gLoggers[name]->set_level(sinkPtr->level());
                // set default log format
                m_gLoggers[name]->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%t] [%l] %v");
            }
        }
    }

    return *m_gLoggers[name];
}