#include "DeviceControl.h"

#include <QDebug>
#include <QProcess>

#include "Constants.h"
#include "EventLog.h"
#include "Settings.h"

namespace
{

void prepareEvemuCommand(QStringList& arguments, const QString& command, bool isNextCommand = true)
{
    if (isNextCommand) {
        arguments << erdc::constants::strings::BashAnd;
        arguments << erdc::constants::strings::BashSleep.arg(QStringLiteral("0.1"));
        arguments << erdc::constants::strings::BashAnd;
    }

    arguments << command;
}

}   // namespace

namespace erdc
{

DeviceControl::DeviceControl(Settings* settingsHandler, EventLog* eventLogHandler, QObject* parent)
    : QObject(parent)
    , mSettingsHandler{ settingsHandler }
    , mEventLogHandler{ eventLogHandler }
{
    assert(settingsHandler);
    assert(eventLogHandler);
}

void DeviceControl::mouseEvent(const QVariantMap& parameters)
{
    if (mIsProcessingEvent) {
        mEventLogHandler->logEvent(constants::enums::EventLogSeverity::Warning,
                                   QStringLiteral("Cannot process mouse event: another processing is still active"));

        return;
    }

    auto event = std::make_shared<MouseEvent>();

    event->deviceName = mSettingsHandler->settings()->value(constants::strings::MouseDeviceName).toString();
    event->requestedPosition = parameters[constants::strings::Position].toPoint();
    event->scale = QPointF(mSettingsHandler->settings()->value(constants::strings::XScaleFactor).toFloat(),
                           mSettingsHandler->settings()->value(constants::strings::YScaleFactor).toFloat());
    event->resultingPosition = QPoint(std::floor(event->requestedPosition.x() * event->scale.x()),
                                      std::floor(event->requestedPosition.y() * event->scale.y()));

    event->mouseEventType = parameters[constants::strings::LeftClick].toBool() ?
                                constants::enums::MouseEventType::LeftClick :
                                constants::enums::MouseEventType::Move;

    QStringList eventArguments;

    // reset postion for absolute movement
    prepareEvemuCommand(
        eventArguments, constants::strings::EV_REL_REL_X.arg(event->deviceName, constants::strings::ZERO_X), false);
    prepareEvemuCommand(eventArguments,
                        constants::strings::EV_REL_REL_Y.arg(event->deviceName, constants::strings::ZERO_Y));

    // move the mouse
    prepareEvemuCommand(
        eventArguments,
        constants::strings::EV_REL_REL_X.arg(event->deviceName, QString::number(event->resultingPosition.x())));
    prepareEvemuCommand(
        eventArguments,
        constants::strings::EV_REL_REL_Y.arg(event->deviceName, QString::number(event->resultingPosition.y())));

    if (parameters[constants::strings::LeftClick].toBool()) {
        // lmb click
        prepareEvemuCommand(eventArguments,
                            constants::strings::EV_KEY_BTN_LEFT.arg(event->deviceName, QStringLiteral("1")));
        prepareEvemuCommand(eventArguments,
                            constants::strings::EV_KEY_BTN_LEFT.arg(event->deviceName, QStringLiteral("0")));
    }

    launchSSHProcess(eventArguments, std::move(event));
}

void DeviceControl::keyboardEvent(int key, int modifiers)
{
    if (mIsProcessingEvent) {
        mEventLogHandler->logEvent(constants::enums::EventLogSeverity::Warning,
                                   QStringLiteral("Cannot process keyboard event: another processing is still active"));

        return;
    }

    const auto parsedKey = parseKeyboardCode(QString(QMetaEnum::fromType<Qt::Key>().valueToKey(key)));

    auto event = std::make_shared<KeyboardEvent>();

    event->deviceName = mSettingsHandler->settings()->value(constants::strings::KeyboardDeviceName).toString();
    event->key = key;
    event->parsedKey = parsedKey;
    event->modifiers = modifiers;

    QStringList eventArguments;

    prepareEvemuCommand(eventArguments,
                        constants::strings::EV_KEY_KEYBOARD.arg(event->deviceName, parsedKey, QStringLiteral("1")),
                        false);

    prepareEvemuCommand(eventArguments,
                        constants::strings::EV_KEY_KEYBOARD.arg(event->deviceName, parsedKey, QStringLiteral("0")));

    launchSSHProcess(eventArguments, std::move(event));
}

bool DeviceControl::isProcessingEvent() const
{
    return mIsProcessingEvent;
}

void DeviceControl::launchSSHProcess(const QStringList& eventArguments, std::shared_ptr<Event>&& event)
{
    if (mSettingsHandler->settings()->value(constants::strings::Password).toString().isEmpty() &&
        mSettingsHandler->settings()->value(constants::strings::IdentityFile).toString().isEmpty()) {

        mEventLogHandler->logEvent(constants::enums::EventLogSeverity::Warning,
                                   QStringLiteral("Credentials not set - check settings"));

        return;
    }

    auto* process = new QProcess(this);

    connect(process, &QProcess::started, this, [this]() { setIsProcessingEvent(true); });

    connect(process,
            &QProcess::finished,
            this,
            [this, process = process, event = event](int exitCode, QProcess::ExitStatus exitStatus) {
                if (exitCode == 0) {
                    mEventLogHandler->logEvent(constants::enums::EventLogSeverity::Info,
                                               QStringLiteral("Event processed, event: %1").arg(event->toString()));
                } else {
                    const auto stderr = process->readAllStandardError();

                    mEventLogHandler->logEvent(
                        constants::enums::EventLogSeverity::Error,
                        QStringLiteral("Event processing failed: %1, event: %2")
                            .arg(stderr.isEmpty() ? QStringLiteral("UNKNOWN ERROR") : stderr, event->toString()));
                }

                process->deleteLater();

                setIsProcessingEvent(false);
            });

    connect(process, &QProcess::errorOccurred, this, [this, event = event](QProcess::ProcessError error) {
        mEventLogHandler->logEvent(constants::enums::EventLogSeverity::Error,
                                   QStringLiteral("Process error occuerd %1:").arg(error));
    });

    QStringList arguments;

    arguments << QStringLiteral("-p") << mSettingsHandler->settings()->value(constants::strings::Password).toString();
    arguments << QStringLiteral("ssh") << QStringLiteral("-l");
    arguments << mSettingsHandler->settings()->value(constants::strings::User).toString();
    arguments << mSettingsHandler->settings()->value(constants::strings::Host).toString();
    arguments << QStringLiteral("-o") << QStringLiteral("PreferredAuthentications=password");
    arguments << eventArguments;

    process->start(QStringLiteral("sshpass"), arguments);
}

void DeviceControl::setIsProcessingEvent(bool isProcessingEvent)
{
    if (mIsProcessingEvent != isProcessingEvent) {
        mIsProcessingEvent = isProcessingEvent;

        emit isProcessingEventChanged(mIsProcessingEvent);
    }
}

QString DeviceControl::parseKeyboardCode(const QString& qtKey) const
{
    const auto& customKeyMappings = mSettingsHandler->customKeyMapping();

    if (customKeyMappings.contains(qtKey)) {
        return customKeyMappings.value(qtKey);
    }

    return qtKey.toUpper();
}

QString MouseEvent::toString() const
{
    return QStringLiteral(
               "[ deviceName: %1, requested position: %2, scale factor: %3, resulting position: %4, mouse event type: %5 ]")
        .arg(deviceName,
             QStringLiteral("(%1, %2)")
                 .arg(QString::number(requestedPosition.x()), QString::number(requestedPosition.y())),
             QStringLiteral("(%1, %2)").arg(QString::number(scale.x()), QString::number(scale.y())),
             QStringLiteral("(%1, %2)")
                 .arg(QString::number(resultingPosition.x()), QString::number(resultingPosition.y())))
        .arg(QVariant::fromValue(mouseEventType).toString());
}

QString KeyboardEvent::toString() const
{
    return QStringLiteral("[ deviceName: %1, key: %2 (%3), parsed key: %4, modifier: %5 ]")
        .arg(deviceName,
             QString::number(key),
             QMetaEnum::fromType<Qt::Key>().valueToKey(key),
             parsedKey,
             QString::number(modifiers));
}

}   // namespace erdc
