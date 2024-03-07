#include "Codec.h"

#include <QJSEngine>
#include <QTextCodec>

#include <stimmt/TypedArrayHelper.h>

namespace stimmt::extension {

    class TextEncoder : public QObject {
    Q_OBJECT
        Q_PROPERTY(QString encoding READ encoding)
    public:
        Q_INVOKABLE TextEncoder() = default;

    public slots:
        QJSValue encode(const QString &string) {
            auto engine = qjsEngine(this);
            Q_ASSERT(engine);
            return TypedArrayHelper::toUint8Array(engine, string.toUtf8());
        };

    private:
        static QString encoding() {
            return "utf-8";
        }
    };

    class TextDecoder : public QObject {
    Q_OBJECT
        Q_PROPERTY(QString encoding READ encoding)
        Q_PROPERTY(bool fatal READ fatal)
        Q_PROPERTY(bool ignoreBOM READ ignoreBOM)
    public:
        Q_INVOKABLE TextDecoder() : TextDecoder("utf-8", {}) {
        }

        Q_INVOKABLE explicit TextDecoder(const QString &ufLabel) : TextDecoder(ufLabel, {}) {
        }

        Q_INVOKABLE explicit TextDecoder(const QString &ufLabel, const QJSValue &options)
            : m_codec(QTextCodec::codecForName(ufLabel.toUtf8())), m_fatal(options.property("fatal").toBool()),
                  m_ignoreBOM(options.property("ignoreBOM").toBool()) {
            if (!m_codec) {
                auto engine = qjsEngine(this);
                Q_ASSERT(engine);
                engine->throwError(QJSValue::RangeError, "Encoding not supported: " + ufLabel);
            }
        }

    public slots:
        QString decode() {
            return decode({}, {});
        }

        QString decode(const QJSValue &buffer) {
            return decode(buffer, {});
        }

        QString decode(const QJSValue &buffer, const QJSValue &options) {
            auto engine = qjsEngine(this);
            Q_ASSERT(engine);
            auto data = TypedArrayHelper::fromGenericValue(engine, buffer);
            if (state.isNull())
                state.reset(
                        new QTextCodec::ConverterState(m_ignoreBOM ? QTextCodec::IgnoreHeader : QTextCodec::DefaultConversion));
            auto s = m_codec->toUnicode(data, data.size(), state.data());
            if (state->invalidChars && m_fatal) {
                engine->throwError(QJSValue::TypeError, QString("Malformed data encountered when decoding ") + m_codec->name());
                return {};
            }
            if (!options.property("stream").toBool())
                state.reset();
            return s;
            return {};
        }

    private:
        QTextCodec *m_codec;
        QScopedPointer<QTextCodec::ConverterState> state;

        bool m_fatal;
        bool m_ignoreBOM;

        QString encoding() const {
            return m_codec->name();
        }

        bool fatal() const {
            return m_fatal;
        }
        bool ignoreBOM() const {
            return m_ignoreBOM;
        }
    };

    Codec::Codec(QJSEngine *engine, QObject *parent) : QObject(parent) {
        engine->globalObject().setProperty("TextDecoder", engine->newQMetaObject<TextDecoder>());
        engine->globalObject().setProperty("TextEncoder", engine->newQMetaObject<TextEncoder>());
    }

    Codec::~Codec() = default;
} // stimmt::extension

#include "Codec.moc"