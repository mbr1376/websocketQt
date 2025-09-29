#include "logger.h"

Logger::Logger(const QString& filename, const QString& dbPath)
    : m_dateTimeFormat("yyyy-MM-dd HH:mm:ss"),
    m_formatTemplate("{datetime} [{level}] ({action}) {message}"),
    m_minLevel(Level::Debug)
{
#ifdef USE_FILE_LOG
    if (!filename.isEmpty()) {
        m_logFile.setFileName(filename);
        if (m_logFile.open(QIODevice::Append | QIODevice::Text)) {
            m_textStream.setDevice(&m_logFile);
            m_textStream.setEncoding(QStringConverter::Utf8);
        }
    }
#endif
#ifdef USE_DB_LOG
    m_dbEnabled = false;
    if (!dbPath.isEmpty()) {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbPath);

        if (m_db.open()) {
            QSqlQuery query;
            query.exec("CREATE TABLE IF NOT EXISTS logs ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "datetime TEXT, "
                       "level TEXT, "
                       "action TEXT, "
                       "message TEXT)");
            m_dbEnabled = true;
        } else {
            qWarning() << "DB Connection failed:" << m_db.lastError().text();
        }
    }
#endif
}


Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

void Logger::log(Action action, const QString& message, Level level)
{
    if (static_cast<int>(level) < static_cast<int>(m_minLevel)) {
        return;
    }

    m_dateTime = QDateTime::currentDateTime();
    QString datetime = m_dateTime.toString(m_dateTimeFormat);
    QString levelStr = levelToString(level);
    QString actionStr = actionToString(action);

#ifdef USE_FILE_LOG
    if (m_logFile.isOpen()) {
        QString logLine = m_formatTemplate;
        logLine.replace("{datetime}", datetime);
        logLine.replace("{level}", levelStr);
        logLine.replace("{action}", actionStr);
        logLine.replace("{message}", message);

        m_textStream << logLine << "\n";
        m_textStream.flush();
    }
#endif

#ifdef USE_DB_LOG
    if (m_dbEnabled) {
        insertIntoDb(datetime, levelStr, actionStr, message);
    }
#endif
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


#ifdef USE_DB_LOG
    void Logger::insertIntoDb(const QString& datetime, const QString& level,
                              const QString& action, const QString& message)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO logs (datetime, level, action, message) VALUES (?, ?, ?, ?)");
        query.addBindValue(datetime);
        query.addBindValue(level);
        query.addBindValue(action);
        query.addBindValue(message);

        if (!query.exec()) {
            qWarning() << "Insert log failed:" << query.lastError().text();
        }
    }
#endif

