#pragma once

#include <QObject>

class QQuickItem;

class IReaderPart
{
public:
    virtual ~IReaderPart() {}
};

Q_DECLARE_INTERFACE(IReaderPart, "org.kde.kparts.qml.reader")

class IWriterPart
{
public:
    virtual ~IWriterPart() {}
};

Q_DECLARE_INTERFACE(IWriterPart, "org.kde.kparts.qml.reader")

class QMLPart : public QObject
{

    Q_OBJECT

public:

    virtual QQuickItem* item() { return nullptr; }

};
