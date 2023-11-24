#ifndef STIMMT_COMBOBOX_H
#define STIMMT_COMBOBOX_H

#include <QComboBox>

#include <stimmt/ScriptObject.h>

class QComboBox;

namespace stimmt {
    class ProjectWindowObject;

    class ComboBox : public QComboBox, public ScriptDescriptiveObject {
        Q_OBJECT
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
        explicit ComboBox(QWidget *parent = nullptr);

    public slots:
        void addOption(const QString &option);
        void insertOption(int index, const QString &option);
        void removeOption(int index);
        void clear();

        QJSValue optionAt(int index) const;
    };
}

#endif // STIMMT_COMBOBOX_H
