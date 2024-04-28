#pragma once

#include <QClipboard>
#include <QObject>

namespace erdc
{

class Clipboard : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

    Q_INVOKABLE void copyToSystemClipboard(const QString& data);
};

}   // namespace erdc
