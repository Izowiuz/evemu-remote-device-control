#pragma once

#include <QObject>
#include <QString>

#include "Constants.h"

class QAbstractItemModel;

namespace erdc
{

class EventsModel;

class EventLog : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel* eventsModel READ eventsModel CONSTANT)

public:
    explicit EventLog(QObject* parent = nullptr);

    QAbstractItemModel* eventsModel() const;
    Q_INVOKABLE void clearEvents();
    Q_INVOKABLE void logEvent(constants::enums::EventLogSeverity severity, const QString& message);

private:
    EventsModel* mEventsModel;
};

}   // namespace erdc
