#pragma once

#include <ctime>
#include <fstream>
#include <SFML\System.hpp>
#include "Constants.h"

// A simple class for logging program events out to a file.
class Logger : sf::NonCopyable
{
public:
    enum LogType { DEBUG, INFO, WARN, ERR };
	static Logger *LogInstance;

private:
    std::ofstream logFile;

    // Logs the current time out to the log file.
    void LogTime()
    {
        time_t rawTime;
        struct tm* localTime;
        time(&rawTime);
        localTime = localtime(&rawTime);

        logFile << "[" << (localTime->tm_year + 1900) << "-" << (localTime->tm_mon + 1) << "-" << localTime->tm_mday
            << " " << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << "] ";
    }

    // Retrieve the log type given the enumeration.
    const char* GetLogType(LogType logType)
    {
        switch (logType)
        {
        case LogType::DEBUG:
            return "debug: ";
        case LogType::INFO:
            return "info: ";
        case LogType::WARN:
            return "WARNING: ";
        case LogType::ERR:
            return "ERROR: ";
        }

        return "EXTREME ERROR: ";
    }

public:
    // Creates and logs the startup text
    Logger(const char* fileName)
    {
        logFile.open(fileName);

        if (logFile.is_open())
        {
            logFile << "Application Starting..." << std::endl;
        }
    }

    // Logs an informational message
    void LogInternal(const char* message)
    {
        LogInternal(LogType::INFO, message);
    }

    // Logs a message out the logger
    void LogInternal(LogType logType, const char* message)
    {
        LogTime();
        logFile << GetLogType(logType) << message << std::endl;
    }

    // Logs a message out the logger with an error code for diagnosis
    void LogInternal(LogType logType, const char* message, int errorCode)
    {
        LogTime();
        logFile << GetLogType(logType) << message << ": " << errorCode << std::endl;
    }

    static void Log(const char* message)
    {
        Logger::LogInstance->LogInternal(message);
    }

    static void LogError(const char* message)
    {
        Logger::LogInstance->LogInternal(LogType::ERR, message);
    }

    static void LogErrorCode(const char* message, int errorCode)
    {
        Logger::LogInstance->LogInternal(LogType::ERR, message, errorCode);
    }

    static void Setup()
    {
        LogInstance = new Logger("temper-fine.log");
    }

    static void Shutdown()
    {
        delete LogInstance;
    }

    // Destructs the logger
    ~Logger()
    {
        if (logFile.is_open())
        {
            logFile << std::endl;
            logFile.close();
        }
    }
};
