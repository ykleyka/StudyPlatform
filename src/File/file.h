#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class File {
protected:
    QString filePath;

public:
    File(const QString& path);
    virtual ~File() = default;

    bool exists() const;
    bool create();
    QString getPath() const { return filePath; }

protected:
    bool openRead(QFile& file, QTextStream& stream);
    bool openWrite(QFile& file, QTextStream& stream, QIODevice::OpenMode mode = QIODevice::WriteOnly);
    bool openAppend(QFile& file, QTextStream& stream);
};

#endif // FILE_H
