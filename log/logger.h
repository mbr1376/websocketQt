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
    Logger(const QString& filename = "log.txt" );
    ~Logger();
    void log(const QString& message);


private:
    QFile m_logFile;
    QTextStream m_textStream;
    QDateTime m_dateTime;

};

#endif