#ifndef STIMMT_FORMLAYOUT_H
#define STIMMT_FORMLAYOUT_H

#include <QFormLayout>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class FormLayout : public QFormLayout, public ScriptDescriptiveObject {
        Q_OBJECT
        Q_PROPERTY(int rowCount READ rowCount)
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
        explicit FormLayout(QWidget *parent = nullptr);

    public slots:
        void addRow(const QString &label, const QJSValue &jsWidget);
        void addElement(const QJSValue &jsWidget);

        void insertRow(int row, const QString &label, const QJSValue &jsWidget);
        void insertElement(int row, const QJSValue &jsWidget);
    };
}

#endif // STIMMT_FORMLAYOUT_H
