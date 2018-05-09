#include "Logger.hpp"

//-------- Logger stream

LoggerOSStream::LoggerOSStream(eLogType level)
{
    type = level;
}

LoggerOSStream::LoggerOSStream(const LoggerOSStream& os)
{
    type = os.type;
}

LoggerOSStream::~LoggerOSStream()
{
    Logger::Instance().log(type, str());
}


//---------



Logger::~Logger()
{
    if(m_printThread.joinable())
        m_printThread.join();

    std::string out;
    for(unsigned int i = 0; i < m_queue.size(); i++)
    {
        m_queue.pop(out);
        std::cout << out << std::endl;
    }
}

Logger& Logger::Instance()
{
    static Logger* m_singleton;
    if(m_singleton == nullptr)
    {
        m_singleton = new Logger();
        m_singleton->m_printThread = std::thread(&Logger::printToConsoleLoop, m_singleton);
    }
    return *m_singleton;
}

LoggerOSStream Logger::Get(eLogType type)
{
    return LoggerOSStream(type);
}


void Logger::log(eLogType level, std::string msg)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if(level > m_displayType)
        return;

    //Format the log msg
    std::ostringstream ss;
    std::time_t t = time(nullptr);
    std::tm *ltm = std::localtime(&t);
    ss << "[";
    ss << ltm->tm_mday;
    ss << "/";
    ss << ltm->tm_mon;
    ss << "/";
    ss << 1900 + ltm->tm_year;
    ss << " - ";
    ss << ltm->tm_hour;
    ss << ":";
    ss << ltm->tm_min;
    ss << ":";
    ss << ltm->tm_sec;
    ss << "]";
    ss << "[" << this->convertEnumToString(level) << "]";
    ss << " ";
    ss << msg;

    m_queue.push(ss.str());
}

void Logger::SetLogerLevel(eLogType type)
{
    m_displayType = type;
}

void Logger::printToConsoleLoop()
{
    while(m_running)
    {
        std::string out;
        m_queue.pop(out);
        std::cout << out << std::endl;
    }
}

std::string Logger::convertEnumToString(eLogType type)
{
    switch(type){
        case l_DEBUG: return "DEBUG";
        case l_INFO: return "INFO";
        case l_WARN: return "WARN";
        case l_ERROR: return "ERROR";
        case l_CRITICAL: return "CRITICAL";
    }
    return "????";
}

eLogType Logger::convertStringToEnum(const std::string& type)
{
    if(type == "DEBUG"){
        return l_DEBUG;
    } else if(type == "INFO"){
        return l_INFO;
    } else if(type == "WARN"){
        return l_WARN;
    } else if(type == "ERROR"){
        return l_ERROR;
    } else if(type == "CRITICAL"){
        return l_CRITICAL;
    }
    return l_INFO;
}
