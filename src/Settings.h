#pragma once

#include <QObject>
#include <QQmlPropertyMap>
#include <QSet>
#include <QSettings>

class QQmlPropertyMap;

namespace erdc
{

class EventLog;

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlPropertyMap* settings READ settings CONSTANT)

public:
    explicit Settings(EventLog* eventLogHandler, QObject* parent = nullptr);

    QQmlPropertyMap* settings() const;
    bool settingsSet() const;
    const QHash<QString, QString>& customKeyMapping() const;

private:
    QQmlPropertyMap* mSettingsMap;
    EventLog* mEventLogHandler;
    QHash<QString, QString> mCustomKeyMappings;

    template <typename T>
    void loadValueIfExists(const QSettings& settings, const QString& key, const T& value)
    {
        if (settings.contains(key)) {
            mSettingsMap->insert(key, settings.value(key).value<T>());
        } else {
            ensureValueInSettingsMap(key, value);
        }
    }

    void saveValue(QSettings& settings, const QString& key);
    void ensureValueInSettingsMap(const QString& key, const QVariant& value);
    bool valueValid(const QVariant& value) const;
    void parseCustomKeyboardMappings(const QVariant &value);
};

}   // namespace erdc

Q_DECLARE_METATYPE(QQmlPropertyMap*)
