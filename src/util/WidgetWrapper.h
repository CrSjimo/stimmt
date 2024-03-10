#ifndef STIMMT_WIDGETWRAPPER_H
#define STIMMT_WIDGETWRAPPER_H

#include <QObject>
#include <QJSValue>

#include <stimmt/stimmtGlobal.h>

class QWidget;
class QLayout;

namespace stimmt {

    namespace modul {
        class Widgets;
    }

    class WidgetWrapperPrivate;

    class STIMMT_EXPORT WidgetWrapper : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(WidgetWrapper)
        Q_PROPERTY(QString id READ id WRITE setId)
        Q_PROPERTY(QString tagName READ tagName)
        Q_PROPERTY(QJSValue children READ children)
    public:
        explicit WidgetWrapper(const QString &tagName, QObject *wrappedObject, QObject *parent = nullptr);
        ~WidgetWrapper() override;

        QObject *wrappedObject() const;

        virtual void render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule);

        QString id() const;
        void setId(const QString &id);

        QString tagName() const;

        virtual QJSValue children() const;

        static inline QObject *unwrapFromJSObject(const QJSValue &obj) {
            auto wrapper = qobject_cast<WidgetWrapper *>(obj.toQObject());
            if (wrapper)
                return wrapper->wrappedObject();
            return nullptr;
        }

    public slots:
        QJSValue findChildById(const QString &id);
        QJSValue findChildrenByTagName(const QString &tagName);

    protected:
        WidgetWrapper(QObject *parent, WidgetWrapperPrivate &d);

    private:
        QScopedPointer<WidgetWrapperPrivate> d_ptr;
    };

} // stimmt

#endif //STIMMT_WIDGETWRAPPER_H
