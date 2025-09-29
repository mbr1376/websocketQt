#ifndef LOGGER_H
#define LOGGER_H

#pragma once


#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

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
    Logger(const QString& filename = "log.txt" );
    ~Logger();
    void log(Action action,const QString& message,Level level = Level::Info);
    void setDateTimeFormat(const QString& format);
    void setFormatTemplate(const QString& format);
    void setMinLevel(Level level);


private:
    QString levelToString(Level level) const;
    QString actionToString(Action action) const;

    QFile m_logFile;
    QTextStream m_textStream;
    QString m_dateTimeFormat;
    QString m_formatTemplate;
    QDateTime m_dateTime;
    Level m_minLevel;

};

#endif
