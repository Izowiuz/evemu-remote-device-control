#pragma once

#include <QtCore/QtCore>

namespace erdc::constants
{
Q_NAMESPACE

namespace enums
{
Q_NAMESPACE

enum class EventLogSeverity { Info, Warning, Error };
Q_ENUM_NS(EventLogSeverity);

enum class MouseEventType { Move, LeftClick };
Q_ENUM_NS(MouseEventType)

}   // namespace enums

namespace strings
{
Q_NAMESPACE

static inline QString MouseDeviceName = QStringLiteral("mouseDeviceName");
static inline QString KeyboardDeviceName = QStringLiteral("keyboardDeviceName");
static inline QString DeviceWidth = QStringLiteral("deviceWidth");
static inline QString DeviceHeight = QStringLiteral("deviceHeight");
static inline QString Host = QStringLiteral("host");
static inline QString User = QStringLiteral("user");
static inline QString IdentityFile = QStringLiteral("identityFile");
static inline QString SshAskPass = QStringLiteral("sshAskPass");
static inline QString Password = QStringLiteral("password");
static inline QString XScaleFactor = QStringLiteral("xScaleFactor");
static inline QString YScaleFactor = QStringLiteral("yScaleFactor");
static inline QString CustomKeyboardMappings = QStringLiteral("customKeyboardMappings");
static inline QString Position = QStringLiteral("position");
static inline QString LeftClick = QStringLiteral("leftClick");
static inline QString BashSleep = QStringLiteral("sleep %1");
static inline QString BashAnd = QStringLiteral(" && ");
static inline QString EV_REL_REL_X = QStringLiteral("evemu-event %1 --type EV_REL --code REL_X --value %2 --sync");
static inline QString EV_REL_REL_Y = QStringLiteral("evemu-event %1 --type EV_REL --code REL_Y --value %2 --sync");
static inline QString EV_KEY_BTN_LEFT =
    QStringLiteral("evemu-event %1 --type EV_KEY --code BTN_LEFT --value %2 --sync");
static inline QString EV_KEY_KEYBOARD = QStringLiteral("evemu-event %1 --type EV_KEY --code %2 --value %3 --sync");
static inline QString ZERO_X = QStringLiteral("-100000");
static inline QString ZERO_Y = QStringLiteral("-100000");
static inline QString Zero = QStringLiteral("0");
static inline QString One = QStringLiteral("1");
static inline QString Qt = QStringLiteral("qt");
static inline QString Ev = QStringLiteral("ev");
static inline QString SshAskPassEnv = QStringLiteral("SSH_ASKPASS");
static inline QString SshAskPassRequireEnv = QStringLiteral("SSH_ASKPASS_REQUIRE");
static inline QString Force = QStringLiteral("force");

}   // namespace strings

};   // namespace erdc::constants
