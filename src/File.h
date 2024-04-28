#pragma once

#include <QObject>
#include <QByteArray>
#include <QUrl>

namespace erdc
{

class File : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

    Q_INVOKABLE QByteArray readFile(const QUrl& path) const;
};

}   // namespace erdc
