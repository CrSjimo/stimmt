#ifndef STIMMT_WIDGETS_H
#define STIMMT_WIDGETS_H

#include <QObject>
#include <QJSValue>

#include <stimmt/stimmtGlobal.h>

namespace stimmt {
    class ModuleSystem;
}

namespace stimmt::modul {

    class WidgetsPrivate;

    class STIMMT_EXPORT Widgets : public QObject {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Widgets)
    public:
        explicit Widgets(QJSEngine *engine, QObject *parent = nullptr);
        ~Widgets() override;

        static void registerModule(ModuleSystem *moduleSystem, Widgets *moduleObject);

        template <typename T>
        void registerWidgetClass(const QString &name) {
            addWidgetClassImpl(&T::staticMetaObject, name);
        }

        template <typename T>
        void registerWidgetClass() {
            addWidgetClassImpl(&T::staticMetaObject, QString(T::staticMetaObject.className()).split("::").last());
        }

        void addExternalWidgetClass(const QJSValue &constructor, QString &name);

    public slots:
        QJSValue render(const QJSValue &description) const;
        QJSValue renderXml(const QString &xmlText) const;

    protected:
        Widgets(QObject *parent, WidgetsPrivate &d);

    private:
        QScopedPointer<WidgetsPrivate> d_ptr;

        void addWidgetClassImpl(const QMetaObject *metaObject, const QString &name);
    };

} // stimmt::modul

#endif //STIMMT_WIDGETS_H
