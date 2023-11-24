#ifndef STIMMT_BUTTONGROUP_H
#define STIMMT_BUTTONGROUP_H

#include <QButtonGroup>
#include <QJSValue>

namespace stimmt {
    class ButtonGroup : public QButtonGroup {
        Q_OBJECT;

    public:
        explicit ButtonGroup(QObject *parent = nullptr);

    public slots:
        void addButton(const QJSValue &jsButton, int id = -1);
        int checkedId() const;
        int id(const QJSValue &jsButton) const;
        void removeButton(const QJSValue &jsButton);
        void setId(const QJSValue &jsButton, int id = -1);
    };
}

#endif // STIMMT_BUTTONGROUP_H
