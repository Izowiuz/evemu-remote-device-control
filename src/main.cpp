#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Clipboard.h"
#include "Constants.h"
#include "DeviceControl.h"
#include "EventLog.h"
#include "File.h"
#include "Settings.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("izowiuz");
    QCoreApplication::setOrganizationDomain("pandemonium");
    QCoreApplication::setApplicationName("remote-device-control");

    QQmlApplicationEngine engine;

    erdc::File file(&app);
    erdc::EventLog eventlog(&app);
    erdc::Clipboard clipboard(&app);
    erdc::Settings settings(&eventlog, &app);
    erdc::DeviceControl deviceControl(&settings, &eventlog, &app);

    // qml register stuff
    qmlRegisterSingletonInstance("ERDC", 1, 0, "FileAPI", &file);
    qmlRegisterSingletonInstance("ERDC", 1, 0, "EventLogAPI", &eventlog);
    qmlRegisterSingletonInstance("ERDC", 1, 0, "ClipboardAPI", &clipboard);
    qmlRegisterSingletonInstance("ERDC", 1, 0, "SettingsAPI", &settings);
    qmlRegisterSingletonInstance("ERDC", 1, 0, "DeviceControlAPI", &deviceControl);

    qmlRegisterUncreatableMetaObject(erdc::constants::enums::staticMetaObject, "ERDC", 1, 0, "Enums", "Uncreatable");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ERDC", "MainWindow");

    return app.exec();
}
