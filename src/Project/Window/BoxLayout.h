#ifndef STIMMT_BOXLAYOUT_H
#define STIMMT_BOXLAYOUT_H

#include <QBoxLayout>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class STIMMT_EXPORT BoxLayout : public QBoxLayout, public ScriptDescriptiveObject {
        Q_OBJECT
        Q_PROPERTY(int direction READ direction WRITE setDirection)
        Q_PROPERTY(int count READ count)
    public:
        QJSValue createScriptObject() override;
        void configureThisScriptObjectByDescription(QJSValue wrappedObject, const QJSValue &attributes,
                                                    const QJSValue &children,
                                                    const std::function<QJSValue(const QJSValue &)> &renderer) override;

        explicit BoxLayout(QWidget *parent = nullptr);
        int direction();
        void setDirection(int direction);

    public slots:
        void addElement(const QJSValue &jsWidget, int stretch = 0, int alignment = 0);
        void addSpacing(int size);
        void addStretch(int stretch = 0);
        void addStrut(int size);

        void insertElement(int index, const QJSValue &jsWidget, int stretch = 0, int alignment = 0);
        void insertSpacing(int index, int size);
        void insertStretch(int index, int stretch = 0);
    };
}

#endif // STIMMT_BOXLAYOUT_H
