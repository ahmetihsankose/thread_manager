#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>
#include <mutex>
#include "Exception.h"

class Logger;

enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class LogFormatter
{
public:
    virtual ~LogFormatter() = default;

    virtual std::string format(const std::string &message,
                               const std::string &level,
                               const std::string &timestamp) = 0;
};

class SimpleLogFormatter : public LogFormatter
{
public:
    std::string format(const std::string &message,
                       const std::string &level,
                       const std::string &timestamp) override
    {
        std::stringstream ss;
        ss << "[" << timestamp << "][" << level << "] " << message;
        return ss.str();
    }
};

class LogOutput
{
public:
    virtual ~LogOutput() = default;

    virtual void write(const std::string &message) = 0;
    virtual void write(const std::string &message, LogLevel level) = 0; // Ekle
};

class ConsoleOutput : public LogOutput
{
public:
    void write(const std::string &message) override
    {
        if (!(std::cout << message << std::endl))
        {
            throw ConsoleOutputWriteException("Failed to write message to console");
        }
    }

    void write(const std::string &message, LogLevel level) override
    {
        std::string colorCode;
        switch (level)
        {
        case LogLevel::DEBUG:
            colorCode = "\033[0;36m"; // Cyan
            break;
        case LogLevel::INFO:
            colorCode = "\033[0;32m"; // Green
            break;
        case LogLevel::WARNING:
            colorCode = "\033[0;33m"; // Yellow
            break;
        case LogLevel::ERROR:
            colorCode = "\033[0;31m"; // Red
            break;
        default:
            colorCode = "\033[0m"; // Reset
        }
        std::cout << colorCode << message << "\033[0m" << std::endl; // Reset color after message
    }
};

class FileOutput : public LogOutput
{
public:
    explicit FileOutput(const std::string &filename)
        : logFile(filename, std::ios_base::trunc) // Open file in truncate mode
    {
        if (!logFile.is_open())
        {
            throw std::runtime_error("Lof file cannot be opened : " + filename);
        }
    }

    void write(const std::string &message) override
    {
        if (!(logFile << message << std::endl))
        {
            throw FileOutputWriteException("Failed to write message to log file");
        }
    }

    void write(const std::string &message, LogLevel level) override
    {
        write(message);
    }

private:
    std::ofstream logFile;
};

class Logger
{
public:
    static Logger &getInstance()
    {
        static Logger instance;
        return instance;
    }

    void setFormatter(std::unique_ptr<LogFormatter> formatter)
    {
        this->formatter = std::move(formatter);
    }

    void addOutput(std::unique_ptr<LogOutput> output)
    {
        outputs.push_back(std::move(output));
    }

    void setMinLogLevel(LogLevel level)
    {
        minLogLevel = level;
    }

    template <typename... Args>
    void log(LogLevel level, const std::string &message, Args &&...args)
    {
        std::unique_lock<std::mutex> lock(logMutex);

        if (level < minLogLevel)
        {
            return;
        }

        std::stringstream ss;
        int dummy[] = {0, ((void)(ss << std::forward<Args>(args)), 0)...};
        (void)dummy;
        std::string formattedMessage = ss.str();
        formattedMessage = message + formattedMessage;

        std::string levelStr;
        switch (level)
        {
        case LogLevel::DEBUG:
            levelStr = "DEBUG";
            break;
        case LogLevel::INFO:
            levelStr = "INFO";
            break;
        case LogLevel::WARNING:
            levelStr = "WARNING";
            break;
        case LogLevel::ERROR:
            levelStr = "ERROR";
            break;
        default:
            levelStr = "UNKNOWN";
        }

        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto tm_now = std::localtime(&time_t_now);
        char timeBuffer[64];
        strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm_now);

        formattedMessage = formatter->format(formattedMessage, levelStr, timeBuffer);

        for (auto &output : outputs)
        {

            if (dynamic_cast<ConsoleOutput *>(output.get()))
            {
                output->write(formattedMessage, level);
            }
            else
            {
                output->write(formattedMessage);
            }
        }
    }

private:
    Logger()
    {
        formatter = std::make_unique<SimpleLogFormatter>();
        outputs.push_back(std::make_unique<ConsoleOutput>());
    }

    std::vector<std::unique_ptr<LogOutput>> outputs;
    std::unique_ptr<LogFormatter> formatter;
    LogLevel minLogLevel = LogLevel::DEBUG;
    std::mutex logMutex;

    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};

#define LOG(level, ...) Logger::getInstance().log(level, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) LOG(LogLevel::INFO, __VA_ARGS__)
#define LOG_WARNING(...) LOG(LogLevel::WARNING, __VA_ARGS__)
#define LOG_ERROR(...) LOG(LogLevel::ERROR, __VA_ARGS__)
