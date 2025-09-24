#include "Logger.h"

Logger::Logger(const QString& filename)
    : m_logFile(filename), m_textStream(&m_logFile)
{
    if (m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        m_textStream.setCodec("UTF-8");
    }
}


Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

void Logger::log(const QString& message)
{
    if (m_logFile.isOpen()) {
        m_dateTime = QDateTime::currentDateTime();
        m_textStream << m_dateTime.toString("yyyy-MM-dd HH:mm:ss") << " - " << message << "\n";
        m_textStream.flush();
    }
}