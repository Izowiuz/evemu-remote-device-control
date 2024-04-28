cmake_minimum_required(VERSION 3.16)

project(evemu-remote-device-control VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(
    evemu-remote-device-control-app
    ""
)

target_include_directories(
    evemu-remote-device-control-app
PUBLIC
    "src"
)

qt_add_qml_module(
    evemu-remote-device-control-app
URI
    ERDC
VERSION
    1.0
QML_FILES
    "src/qml/MainWindow.qml"
    "src/qml/RDCUI.qml"
    "src/qml/Config.qml"
    "src/qml/Device.qml"
    "src/qml/FrameLabel.qml"
    "src/qml/KeyboardEmulator.qml"
    "src/qml/EventLog.qml"
    "src/qml/SettingsDialog.qml"
    "src/qml/TextFieldSetting.qml"
    "src/qml/HelpDialog.qml"
)

qt_add_resources(
    evemu-remote-device-control-app
    "resources"
PREFIX
    "/"
FILES
    "qtquickcontrols2.conf"
    "src/Help.md"
)

target_sources(
    evemu-remote-device-control-app
PRIVATE
    "src/main.cpp"
    "src/EventLog.h"
    "src/EventLog.cpp"
    "src/EventsModel.h"
    "src/EventsModel.cpp"
    "src/Constants.h"
    "src/Settings.h"
    "src/Settings.cpp"
    "src/Clipboard.h"
    "src/Clipboard.cpp"
    "src/DeviceControl.h"
    "src/DeviceControl.cpp"
    "src/MouseEvent.h"
    "src/File.h"
    "src/File.cpp"
)

target_link_libraries(
    evemu-remote-device-control-app
PRIVATE
    Qt6::Quick
)

include(GNUInstallDirs)
install(
TARGETS
    evemu-remote-device-control-app
BUNDLE DESTINATION
    .
LIBRARY DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)
