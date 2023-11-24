#ifndef STIMMT_TEXTDECODER_H
#define STIMMT_TEXTDECODER_H

#include <QObject>
#include <QJSValue>
#include <QTextCodec>

#include <stimmt/StimmtGlobal.h>

namespace stimmt {
    class STIMMT_EXPORT TextDecoder : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString encoding READ encoding)
        Q_PROPERTY(bool fatal READ fatal)
        Q_PROPERTY(bool ignoreBOM READ ignoreBOM)
    public:
        Q_INVOKABLE TextDecoder();
        Q_INVOKABLE explicit TextDecoder(const QString &ufLabel);
        Q_INVOKABLE explicit TextDecoder(const QString &ufLabel, const QJSValue &options);

        QString encoding() const;
        bool fatal() const;
        bool ignoreBOM() const;

    public slots:
        QString decode();
        QString decode(const QJSValue &buffer);
        QString decode(const QJSValue &buffer, const QJSValue &options);

    private:
        QTextCodec *m_codec;
        QScopedPointer<QTextCodec::ConverterState> state;

        bool m_fatal;
        bool m_ignoreBOM;
    };
}

#endif // STIMMT_TEXTDECODER_H
