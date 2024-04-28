#include "EventsModel.h"

#include <QDebug>

namespace erdc
{

int EventsModel::rowCount(const QModelIndex& parent) const
{
    return mEvents.size();
}

int EventsModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant EventsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    if (index.row() > mEvents.size()) {
        return {};
    }

    switch (static_cast<RoleNames>(role)) {
    case RoleNames::Timestamp:
        return mEvents[index.row()].timestamp;
    case RoleNames::Severity:
        return QVariant::fromValue(mEvents[index.row()].severity);
    case RoleNames::Message:
        return mEvents[index.row()].message;
    default:
        return {};
    }

    return {};
}

QHash<int, QByteArray> EventsModel::roleNames() const
{
    static QHash<int, QByteArray> roleNamesHash = {
        { static_cast<int>(RoleNames::Timestamp), QByteArrayLiteral("timestamp") },
        { static_cast<int>(RoleNames::Severity), QByteArrayLiteral("severity") },
        { static_cast<int>(RoleNames::Message), QByteArrayLiteral("message") }
    };

    return roleNamesHash;
}

void EventsModel::addEvent(constants::enums::EventLogSeverity severity, const QString& message)
{
    qDebug() << "Incoming event with severity:" << severity << "and message:" << message;

    beginInsertRows({}, mEvents.size(), mEvents.size());
    mEvents.push_back({ QDateTime::currentDateTimeUtc(), severity, message });
    endInsertRows();
}

void EventsModel::clearEvents()
{
    beginResetModel();
    mEvents.clear();
    endResetModel();
}

}   // namespace erdc
