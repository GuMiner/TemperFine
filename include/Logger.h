#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <SFML\System.hpp>
#include "Constants.h"

// A simple class for logging program events out to a file.
class Logger : sf::NonCopyable
{
public:
    enum LogType { INFO, WARN, ERR };
	static Logger *LogStream;

    // Creates and logs the startup text
    Logger(const char* fileName);

    // Various convenient logging methods.
    template<typename T>
    static void Log(T message)
    {
        std::stringstream stringifiedMessage;
        stringifiedMessage << message;
        LogStream->LogInternal(LogType::INFO, stringifiedMessage.str().c_str());
    }

    template<typename T, typename... Remainder>
    static void Log(T message, Remainder... remainder)
    {
        std::string fullMessage = FormFullLogMessage(message, remainder...);
        LogStream->LogInternal(LogType::INFO, fullMessage.c_str());
    }

    template<typename T>
    static void LogWarn(T message)
    {
        std::stringstream stringifiedMessage;
        stringifiedMessage << message;
        LogStream->LogInternal(LogType::WARN, stringifiedMessage.str().c_str());
    }

    template<typename T, typename... Remainder>
    static void LogWarn(T message, Remainder... remainder)
    {
        std::string fullMessage = FormFullLogMessage(message, remainder...);
        LogStream->LogInternal(LogType::WARN, fullMessage.c_str());
    }

    template<typename T>
    static void LogError(T message)
    {
        std::stringstream stringifiedMessage;
        stringifiedMessage << message;
        LogStream->LogInternal(LogType::ERR, stringifiedMessage.str().c_str());
    }

    template<typename T, typename... Remainder>
    static void LogError(T message, Remainder... remainder)
    {
        std::string fullMessage = FormFullLogMessage(message, remainder...);
        LogStream->LogInternal(LogType::ERR, fullMessage.c_str());
    }

    // Control methods for the static Logging instance.
    static void Setup();
    static void Shutdown();

    // Destructs the logger
    ~Logger();

private:
    std::ofstream logFile;
    sf::Mutex writeLock;

    // Recursion finale.
    template<typename T>
    static std::string FormFullLogMessage(T message)
    {
        std::stringstream streamData;
        streamData << message;
        return streamData.str();
    }

     // Uses variadic templates to form a full log message.
    template<typename T, typename... Remainder>
    static std::string FormFullLogMessage(T message, Remainder... remainder)
    {
        std::stringstream streamData;
        streamData << message << Logger::FormFullLogMessage(remainder...);
        return streamData.str();
    }

    // Logs a message out the logger
    void LogInternal(LogType logType, const char* message);

    // Logs the current time out to the log file.
    void LogTime();

    // Retrieve the log type given the enumeration.
    const char* GetLogType(LogType logType);
};
