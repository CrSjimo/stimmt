#ifndef STIMMT_STACKEDLAYOUT_H
#define STIMMT_STACKEDLAYOUT_H

#include <QStackedLayout>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class StackedLayout : public QStackedLayout, public ScriptDescriptiveObject {
        Q_OBJECT
        Q_PROPERTY(int count READ count)
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
        explicit StackedLayout(QWidget *parent = nullptr);

    public slots:
        void addElement(const QJSValue &jsWidget);
        void insertElement(int index, const QJSValue &jsWidget);
    };
}

#endif // STIMMT_STACKEDLAYOUT_H
