#pragma once

#include <QAbstractItemModel>
#include <QDateTime>
#include <QList>

#include "Constants.h"

namespace erdc
{

class EventsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum class RoleNames { Timestamp = Qt::UserRole + 1, Severity, Message };

    using QAbstractListModel::QAbstractListModel;

    // QAbstractListModel interface -->
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    // QAbstractListModel interface <--

    void addEvent(constants::enums::EventLogSeverity severity, const QString& message);
    void clearEvents();

private:
    struct Event {
        QDateTime timestamp;
        constants::enums::EventLogSeverity severity{ constants::enums::EventLogSeverity::Info };
        QString message;
    };

    QList<Event> mEvents;

public:
};

}   // namespace erdc
