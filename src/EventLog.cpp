#include "EventLog.h"

#include <QDebug>

#include "EventsModel.h"

namespace erdc
{

EventLog::EventLog(QObject* parent)
    : mEventsModel(new EventsModel(this))
{
}

QAbstractItemModel* EventLog::eventsModel() const
{
    return mEventsModel;
}

void EventLog::clearEvents()
{
    mEventsModel->clearEvents();
}

void EventLog::logEvent(constants::enums::EventLogSeverity severity, const QString& message)
{
    mEventsModel->addEvent(severity, message);
}

}   // namespace erdc
