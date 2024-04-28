#include "Clipboard.h"

#include <QGuiApplication>

namespace erdc
{

void Clipboard::copyToSystemClipboard(const QString& data)
{
    qApp->clipboard()->setText(data);
}

}   // namespace erdc
