#include "Settings.h"

#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>

#include "Constants.h"
#include "EventLog.h"

namespace erdc
{

Settings::Settings(EventLog* eventLogHandler, QObject* parent)
    : mSettingsMap(new QQmlPropertyMap(this))
    , mEventLogHandler(eventLogHandler)
{
    assert(mEventLogHandler);

    QSettings settings;

    connect(mSettingsMap, &QQmlPropertyMap::valueChanged, this, [this](const QString& key, const QVariant& value) {
        if (key == constants::strings::CustomKeyboardMappings && value.toString() != QStringLiteral("[]")) {
            parseCustomKeyboardMappings(value);
        }
    });

    loadValueIfExists(settings, constants::strings::MouseDeviceName, QStringLiteral(""));
    loadValueIfExists(settings, constants::strings::KeyboardDeviceName, QStringLiteral(""));
    loadValueIfExists(settings, constants::strings::DeviceWidth, 800);
    loadValueIfExists(settings, constants::strings::DeviceHeight, 600);
    loadValueIfExists(settings, constants::strings::Host, QStringLiteral(""));
    loadValueIfExists(settings, constants::strings::User, QStringLiteral(""));
    loadValueIfExists(settings, constants::strings::Password, QStringLiteral(""));
    loadValueIfExists(settings, constants::strings::XScaleFactor, 1.f);
    loadValueIfExists(settings, constants::strings::YScaleFactor, 1.f);
    loadValueIfExists(settings, constants::strings::CustomKeyboardMappings, QStringLiteral("[]"));

    mSettingsMap->freeze();

    parseCustomKeyboardMappings(mSettingsMap->value(constants::strings::CustomKeyboardMappings));

    connect(qApp, &QCoreApplication::aboutToQuit, this, [this]() {
        QSettings settings;

        saveValue(settings, constants::strings::MouseDeviceName);
        saveValue(settings, constants::strings::KeyboardDeviceName);
        saveValue(settings, constants::strings::DeviceWidth);
        saveValue(settings, constants::strings::DeviceHeight);
        saveValue(settings, constants::strings::Host);
        saveValue(settings, constants::strings::User);
        saveValue(settings, constants::strings::XScaleFactor);
        saveValue(settings, constants::strings::YScaleFactor);
        saveValue(settings, constants::strings::CustomKeyboardMappings);
    });
}

QQmlPropertyMap* Settings::settings() const
{
    return mSettingsMap;
}

const QHash<QString, QString>& Settings::customKeyMapping() const
{
    return mCustomKeyMappings;
}

void Settings::saveValue(QSettings& settings, const QString& key)
{
    settings.setValue(key, mSettingsMap->value(key));
}

void Settings::ensureValueInSettingsMap(const QString& key, const QVariant& value)
{
    if (!mSettingsMap->contains(key)) {
        mSettingsMap->insert(key, value);
    }
}

bool Settings::valueValid(const QVariant& value) const
{
    return !value.toString().isEmpty();
}

void Settings::parseCustomKeyboardMappings(const QVariant& value)
{
    mCustomKeyMappings.clear();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(value.toByteArray(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        mEventLogHandler->logEvent(
            constants::enums::EventLogSeverity::Error,
            QStringLiteral("Custom keyboard mappings parse error - mappings will not be valid: %1 - %2")
                .arg(parseError.errorString(), value.toString()));

        return;
    }

    if (!doc.isArray()) {
        mEventLogHandler->logEvent(
            constants::enums::EventLogSeverity::Error,
            QStringLiteral("Custom keyboard mappings have to be an JSON array  - mappings will not be valid")
                .arg(parseError.errorString()));

        return;
    }

    const auto jsonArray = doc.array();
    for (const auto& elem : jsonArray) {
        if (elem.isObject()) {
            const auto& obj = elem.toObject();

            const auto qtKey = obj[constants::strings::Qt].toString();
            const auto evKey = obj[constants::strings::Ev].toString();

            if (!qtKey.isEmpty() && !evKey.isEmpty()) {
                mCustomKeyMappings.insert(qtKey, evKey);
            }
        }
    }

    mEventLogHandler->logEvent(constants::enums::EventLogSeverity::Info,
                               QStringLiteral("Custom keyboard mappings parsed: %1").arg(value.toString()));
}

}   // namespace erdc
