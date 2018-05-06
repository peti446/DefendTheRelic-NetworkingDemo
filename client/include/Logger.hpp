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

        //---- Static functions (Instance will be called inside to make it easier to use)
        static Logger& Instance();
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
        static void SetLogerLevel(eLogType type);


        //Non static function, to call them you need to call Instance() first;
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
#endif // LOGGER_H
