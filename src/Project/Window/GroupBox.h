#ifndef STIMMT_GROUPBOX_H
#define STIMMT_GROUPBOX_H

#include <QGroupBox>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class GroupBox : public QGroupBox, public ScriptDescriptiveObject {
        Q_OBJECT
        Q_PROPERTY(int alignment READ alignment WRITE setAlignment)
        Q_PROPERTY(QJSValue content READ content WRITE setContent)
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;
        explicit GroupBox(QWidget *parent = nullptr);

        void setContent(const QJSValue &jsWidget);
        QJSValue content() const;

    private:
        QJSValue m_content = QJSValue::NullValue;
    };
}

#endif // STIMMT_GROUPBOX_H
