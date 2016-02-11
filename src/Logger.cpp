#include <ctime>
#include "Logger.h"

Logger *Logger::LogStream;

void Logger::Setup()
{
    LogStream = new Logger("temper-fine.log");
}

void Logger::Shutdown()
{
    delete LogStream;
}

// Creates and logs the startup text
Logger::Logger(const char* fileName)
{
    logFile.open(fileName);

    if (logFile.is_open())
    {
        logFile << "Application Starting..." << std::endl;
    }
}

// Logs a message out the logger
void Logger::LogInternal(LogType logType, const char* message)
{
    writeLock.lock();

    LogTime();
    logFile << GetLogType(logType) << message << std::endl;

    writeLock.unlock();
}

// Logs the current time out to the log file.
void Logger::LogTime()
{
    time_t rawTime;
    struct tm* localTime;
    time(&rawTime);
    localTime = localtime(&rawTime);

    logFile << "[" << (localTime->tm_year + 1900) << "-" << (localTime->tm_mon + 1) << "-" << localTime->tm_mday
        << " " << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << "] ";
}

// Retrieve the log type given the enumeration.
const char* Logger::GetLogType(LogType logType)
{
    switch (logType)
    {
    case LogType::INFO:
        return "info: ";
    case LogType::WARN:
        return "warn: ";
    case LogType::ERR:
        return "error: ";
    }

    return "extreme_error: ";
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile << std::endl;
        logFile.close();
    }
}
