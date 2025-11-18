#include "file.h"

File::File(const QString& path) : filePath(path) {}

bool File::exists() const {
    return QFile::exists(filePath);
}

bool File::create() {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot create file:" << filePath;
        return false;
    }
    file.close();
    return true;
}

bool File::openRead(QFile& file, QTextStream& stream) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading:" << filePath;
        return false;
    }
    stream.setDevice(&file);
    return true;
}

bool File::openWrite(QFile& file, QTextStream& stream, QIODevice::OpenMode mode) {
    if (!file.open(mode | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        return false;
    }
    stream.setDevice(&file);
    return true;
}

bool File::openAppend(QFile& file, QTextStream& stream) {
    return openWrite(file, stream, QIODevice::Append);
}
