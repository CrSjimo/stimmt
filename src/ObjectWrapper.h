#ifndef STIMMT_OBJECTWRAPPER_H
#define STIMMT_OBJECTWRAPPER_H

#include <QJSValue>
#include <QObject>
#include <QVariant>

#include <stimmt/StimmtGlobal.h>

class QJSEngine;

namespace stimmt {
    class STIMMT_EXPORT JSValueAccessorDescriptor : public QObject {
        Q_OBJECT;

    public:
        explicit JSValueAccessorDescriptor(QJSValue obj, QString key)
            : QObject(nullptr), m_obj(std::move(obj)), m_key(std::move(key)) {
        }

    public slots:
        QJSValue get() const {
            return m_obj.property(m_key);
        }
        void set(const QJSValue &value) {
            return m_obj.setProperty(m_key, value);
        }

    private:
        QJSValue m_obj;
        QString m_key;
    };

    class STIMMT_EXPORT JSValueCustomAccessorDescriptor : public QObject {
        Q_OBJECT
        using GetterFunc = std::function<QJSValue()>;
        using SetterFunc = std::function<void(const QJSValue &)>;

    public:
        explicit JSValueCustomAccessorDescriptor(const GetterFunc &getter, const SetterFunc &setter)
            : QObject(nullptr), getter(getter), setter(setter) {
        }

    public slots:
        QJSValue get() const {
            return getter();
        }
        void set(const QJSValue &value) {
            setter(value);
        }

    private:
        GetterFunc getter;
        SetterFunc setter;
    };

    class STIMMT_EXPORT ObjectWrapper {
    public:
        static QStringList qWidgetGeneralKeys();

        static QJSValue wrap(QObject *obj, QJSEngine *engine, const QStringList &includedKeys = {},
                             const QStringList &excludedKeys = {});

        template <class Obj, class Ret, class FuncClass, class... Args>
        static void bindSignal(Obj *obj, Ret (FuncClass::*func)(Args...), QJSValue wrapped, const QString &name) {
            QString key = "on" + name;
            key[2] = key[2].toUpper();
            wrapped.setProperty(key, QJSValue::NullValue);
            QObject::connect(obj, func, [=](Args... args) {
                QJSValueList argList = {args...};
                auto cb = wrapped.property(key);
                if (cb.isCallable())
                    cb.call(argList);
            });
        }

        template <typename GetterFunctor, typename SetterFunctor>
        static void addAccessorProperty(QJSValue wrapped, QJSEngine *engine, const QString &key,
                                        GetterFunctor getterFunctor, SetterFunctor setterFunctor) {
            addAccessorPropertyImpl(wrapped, engine, key,
                                    new JSValueCustomAccessorDescriptor(getterFunctor, setterFunctor));
        }

        static QJSValue toUint8Array(const QByteArray &ba, QJSEngine *engine);
        static void fillUint8Array(const QByteArray &ba, QJSValue uint8Array);
        static QByteArray fromUint8Array(const QJSValue &uint8Array, QJSEngine *engine);

        template <class T>
        static T *getOfWrappedObject(const QJSValue &obj) {
            auto *v = qobject_cast<T *>(obj.toQObject());
            if (v)
                return v;
            v = qobject_cast<T *>(obj.property("_p").toQObject());
            if (v)
                return v;
            v = qobject_cast<T *>(obj.property("_p").property("_subObject").toQObject());
            return v;
        }

    private:
        static void addAccessorPropertyImpl(QJSValue wrapped, QJSEngine *engine, const QString &key,
                                            JSValueCustomAccessorDescriptor *descriptorObject);
    };
}

#define OBJECT_WRAPPER_BIND_SIGNAL(obj, wrapped, signal) ObjectWrapper::bindSignal(obj, &std::remove_pointer<decltype(obj)>::type::signal, wrapped, #signal)


#endif // STIMMT_OBJECTWRAPPER_H
