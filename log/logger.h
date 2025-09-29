#ifndef LOGGER_H
#define LOGGER_H

#pragma once


#include <QString>
#include <QDateTime>

#ifdef USE_FILE_LOG
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#endif

#ifdef USE_DB_LOG
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#endif

class Logger
{
public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    enum class Action {
        Init,
        Shutdown,
        UserLogin,
        UserLogout,
        DbConnect,
        DbQuery,
        FileIO,
        Network,
        Process,
        Custom   // برای موارد خاص
    };
    Logger(const QString& filename = QString(), const QString& dbPath = QString());
    ~Logger();
    void log(Action action,const QString& message,Level level = Level::Info);
    void setDateTimeFormat(const QString& format);
    void setFormatTemplate(const QString& format);
    void setMinLevel(Level level);


private:
    QString levelToString(Level level) const;
    QString actionToString(Action action) const;

#ifdef USE_FILE_LOG
    QFile m_logFile;
    QTextStream m_textStream;
#endif

#ifdef USE_DB_LOG
    QSqlDatabase m_db;
    bool m_dbEnabled;
    void insertIntoDb(const QString& datetime, const QString& level, const QString& action, const QString& message);
#endif
    QString m_dateTimeFormat;
    QString m_formatTemplate;
    QDateTime m_dateTime;
    Level m_minLevel;

};

#endif
