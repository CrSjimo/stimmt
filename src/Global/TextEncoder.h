#ifndef STIMMT_TEXTENCODER_H
#define STIMMT_TEXTENCODER_H

#include <QObject>
#include <QJSValue>

#include <stimmt/StimmtGlobal.h>

namespace stimmt {
    class STIMMT_EXPORT TextEncoder : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString encoding READ encoding)
    public:
        Q_INVOKABLE TextEncoder();

        static QString encoding() {
            return "utf-8";
        }

    public slots:
        QJSValue encode(const QString &string);
    };
}

#endif // STIMMT_TEXTENCODER_H
