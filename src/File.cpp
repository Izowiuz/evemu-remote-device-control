#include "File.h"

#include <QFile>

namespace erdc
{

QByteArray File::readFile(const QUrl& path) const
{
    QFile file(path.path());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }

    return file.readAll();
}

}   // namespace erdc
