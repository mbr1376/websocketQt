#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>

class Context : public QObject
{
    Q_OBJECT
public:
    explicit Context(QObject *parent = nullptr);

signals:
};

#endif // CONTEXT_H
