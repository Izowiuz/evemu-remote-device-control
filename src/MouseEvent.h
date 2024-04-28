#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

// namespace erdc
// {

class Actor
{
    Q_GADGET
    QML_VALUE_TYPE(actor)
    QML_CONSTRUCTIBLE_VALUE
    Q_PROPERTY(QString name READ name WRITE setName)

public:
    Q_INVOKABLE Actor() = default;

    QString name() const
    {
        return m_name;
    }

    void setName(const QString& name)
    {
        m_name = name;
    }

private:
    QString m_name;
};

// }   // namespace erdc
