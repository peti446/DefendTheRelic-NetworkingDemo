#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <thread>
#include <sstream>
#include <string>
#include <iostream>
#include "ConcurrentQueue.hpp"

enum eLogType
{
    l_CRITICAL,
    l_ERROR,
    l_WARN,
    l_INFO,
    l_DEBUG,
};

//For the singleton to work we need to forwarde declare it
class Logger;

class LoggerOSStream : public std::ostringstream
{
    public:
        LoggerOSStream(eLogType level = l_DEBUG);
        LoggerOSStream(const LoggerOSStream& os);
        ~LoggerOSStream();
    private:
        eLogType type;
};

class Logger
{
    public:
        //Delete copy and move constructors
        Logger(Logger const& c) = delete;
        void operator=(Logger const& c) = delete;

        //Destructor
        ~Logger();

        //---- Static Singleton
        static Logger& Instance();
        //-----
        /*
        *   Function to get a LoggerOSStream, its static so we can easly access it
        */
        static LoggerOSStream Get(eLogType type = l_DEBUG);

        /*
        *   Sets what type of log messages should be printed and wich ones should not.
        *   By default the level is set to DEBUG,  which means that every logged msg will be printed.
        *   If we set a log type to higer one in the tree, every type below the selected one, will not be printed,
        *   and everything above it will be.
        *   Hierarchy:
        *   - l_CRITICAL
        *   - l_ERROR
        *   - l_WARN
        *   - l_INFO
        *   - l_DEBUG
        */
        void SetLogerLevel(eLogType type);

        void log(eLogType level, std::string msg);
    private:
        //Functions
        //Cannot build
        Logger() = default;
        void printToConsoleLoop();
        //Helper functions
        std::string convertEnumToString(eLogType type);
        eLogType convertStringToEnum(const std::string& type);

        //Variables
        std::mutex m_mutex;
        ConcurrentQueue<std::string> m_queue;
        bool m_running{true};
        std::thread m_printThread;
        eLogType m_displayType{eLogType::l_DEBUG};
};

//Define
typedef Logger Log;
#define Log(level) Logger::Get(level)

#endif // LOGGER_H
