//#include "Logger.h"
//
//BAE::Logger::Logger(const std::string& fileName) : _logFile(fileName, std::ios::app), _exitFlag(false) {
//    _writerThread = std::thread(&Logger::ProcessLogs, this);
//}
//
//BAE::Logger::~Logger()
//{
//    _exitFlag = true;
//    _condVar.notify_one();
//    _writerThread.join();
//}
//
//void BAE::Logger::Log(const std::string& message, LogLevel level)
//{
//    std::unique_lock<std::mutex> lock(_queueMutex);
//    _logQueue.push("[" + LogLevelToString(level) + "] " + message);
//    _condVar.notify_one();
//}
//
//void BAE::Logger::ProcessLogs()
//{
//    while (true) {
//        std::unique_lock<std::mutex> lock(_queueMutex);
//        _condVar.wait(lock, [this] { return !_logQueue.empty() || _exitFlag; });
//
//        if (_exitFlag && _logQueue.empty()) break;
//
//        while (!_logQueue.empty()) {
//            _logFile << _logQueue.front() << std::endl;
//            _logQueue.pop();
//        }
//    }
//}
//
//std::string BAE::Logger::LogLevelToString(LogLevel level)
//{
//    switch (level) {
//    case LogLevel::DEBUG: return "DEBUG";
//    case LogLevel::INFO: return "INFO";
//    case LogLevel::WARNING: return "WARNING";
//    case LogLevel::ERROR: return "ERROR";
//    default: return "UNKNOWN";
//    }
//}

#include "Logger.h"
#include <iostream>

using namespace BAE;

std::mutex Logger::_mutex;

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : _exitFlag(false), _logFile("log.txt", std::ios::app) {
    _writerThread = std::thread(&Logger::ProcessLogs, this);
}

Logger::~Logger() {
    _exitFlag = true;
    _condVar.notify_one();
    _writerThread.join();
    _logFile.close();
}

void Logger::Log(LogLevel level, const std::string& message) {
    return;
    std::unique_lock<std::mutex> lock(_mutex);
    _logQueue.push("[" + LogLevelToString(level) + "] " + message);
    _condVar.notify_one();
}

std::string Logger::LogLevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::Debug: return "Debug";
    case LogLevel::Info: return "Info";
    case LogLevel::Warning: return "Warning";
    case LogLevel::Error: return "Error";
    default: return "Unknown";
    }
}

void Logger::ProcessLogs() {
    while (true) {
        std::unique_lock<std::mutex> lock(_mutex);
        _condVar.wait(lock, [this] { return !_logQueue.empty() || _exitFlag; });

        while (!_logQueue.empty()) {
            _logFile << _logQueue.front() << std::endl;
            _logQueue.pop();
        }

        if (_exitFlag) break;
    }
}
