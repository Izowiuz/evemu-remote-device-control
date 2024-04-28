#pragma once

#include <QObject>
#include <QPointF>
#include <QVariantMap>

#include <QDebug>

#include <memory>

#include "Constants.h"

namespace erdc
{

class Settings;
class EventLog;

struct Event {
    virtual QString toString() const = 0;

    QString deviceName;
};

struct MouseEvent final : public Event {
    QPoint requestedPosition;
    QPointF scale;
    QPoint resultingPosition;
    constants::enums::MouseEventType mouseEventType{ constants::enums::MouseEventType::Move };

    QString toString() const override;
};

struct KeyboardEvent : public Event {
    int key{0};
    QString parsedKey;
    int modifiers{0};

    QString toString() const override;
};

class DeviceControl : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isProcessingEvent READ isProcessingEvent NOTIFY isProcessingEventChanged)

public:
    DeviceControl(Settings* settingsHandler, EventLog* eventLogHandler, QObject* parent = nullptr);

    Q_INVOKABLE void mouseEvent(const QVariantMap& parameters);
    Q_INVOKABLE void keyboardEvent(int key, int modifiers);
    bool isProcessingEvent() const;

private:
    Settings* mSettingsHandler;
    EventLog* mEventLogHandler;
    bool mIsProcessingEvent{ false };

    void launchSSHProcess(const QStringList& eventArgument, std::shared_ptr<Event>&& event);
    void setIsProcessingEvent(bool isProcessingEvent);
    QString parseKeyboardCode(const QString& qtKey) const;

signals:
    void isProcessingEventChanged(bool isProcessingEvent);
};

}   // namespace erdc
