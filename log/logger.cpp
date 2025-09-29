#include "logger.h"

Logger::Logger(const QString& filename)
    : m_logFile(filename), m_textStream(&m_logFile)
{
    if (m_logFile.open(QIODevice::Append | QIODevice::Text)) {

        m_textStream.setEncoding(QStringConverter::Utf8);
    }
}


Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

void Logger::log(Action action, const QString& message, Level level)
{
    if (m_logFile.isOpen()) {
        m_dateTime = QDateTime::currentDateTime();
        m_textStream << m_dateTime.toString("yyyy-MM-dd HH:mm:ss") << " - " << message << "\n";
        m_textStream.flush();
    }
}

void Logger::setDateTimeFormat(const QString &format)
{
    m_dateTimeFormat  = format;
}

void Logger::setFormatTemplate(const QString &format)
{
    m_formatTemplate = format;
}

void Logger::setMinLevel(Level level)
{
    m_minLevel = level;
}

QString Logger::levelToString(Level level) const
{
    switch (level) {
    case Level::Debug:    return "DEBUG";
    case Level::Info:     return "INFO";
    case Level::Warning:  return "WARNING";
    case Level::Error:    return "ERROR";
    case Level::Critical: return "CRITICAL";
    }
    return "UNKNOWN";
}

QString Logger::actionToString(Action action) const
{
    switch (action) {
    case Action::Init:       return "INIT";
    case Action::Shutdown:   return "SHUTDOWN";
    case Action::UserLogin:  return "USER_LOGIN";
    case Action::UserLogout: return "USER_LOGOUT";
    case Action::DbConnect:  return "DB_CONNECT";
    case Action::DbQuery:    return "DB_QUERY";
    case Action::FileIO:     return "FILE_IO";
    case Action::Network:    return "NETWORK";
    case Action::Process:    return "PROCESS";
    case Action::Custom:     return "CUSTOM";
    }
    return "UNKNOWN";
}
