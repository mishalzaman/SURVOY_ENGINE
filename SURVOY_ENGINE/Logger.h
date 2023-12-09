#pragma once

//#include <fstream>
//#include <string>
//#include <mutex>
//#include <queue>
//#include <thread>
//#include <condition_variable>
//#include <atomic>
//
//namespace BAE {
//
//    enum class LogLevel {
//        DEBUG, INFO, WARNING, ERROR
//    };
//
//    class Logger {
//    public:
//        Logger(const std::string& fileName);
//
//        ~Logger();
//
//        void Log(const std::string& message, LogLevel level = LogLevel::INFO);
//
//    private:
//        std::ofstream _logFile;
//        std::mutex _queueMutex;
//        std::queue<std::string> _logQueue;
//        std::thread _writerThread;
//        std::condition_variable _condVar;
//        std::atomic<bool> _exitFlag;
//
//        void ProcessLogs();
//
//        std::string LogLevelToString(LogLevel level);
//    };
//
//} // namespace BAE

#include <fstream>
#include <string>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace BAE {
    // Verbosity Levels
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    // Logger Class
    class Logger {
    public:
        static Logger& Instance();
        ~Logger();

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        void Log(LogLevel level, const std::string& message);

        // Additional public interface if needed...

    private:
        Logger();
        static std::string LogLevelToString(LogLevel level);
        void ProcessLogs();

        static std::mutex _mutex;
        std::queue<std::string> _logQueue;
        std::condition_variable _condVar;
        std::atomic<bool> _exitFlag;
        std::thread _writerThread;
        std::ofstream _logFile;
    };
}

// Logging Macros
#define LOG_DEBUG(message) Logger::Instance().Log(LogLevel::Debug, message)
#define LOG_INFO(message) Logger::Instance().Log(LogLevel::Info, message)
#define LOG_WARNING(message) Logger::Instance().Log(LogLevel::Warning, message)
#define LOG_ERROR(message) Logger::Instance().Log(LogLevel::Error, message)