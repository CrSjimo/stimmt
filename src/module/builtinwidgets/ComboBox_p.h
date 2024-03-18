#ifndef STIMMT_COMBOBOX_P_H
#define STIMMT_COMBOBOX_P_H

#include <QComboBox>

#include <stimmt/WidgetWrapper.h>

namespace stimmt::widgets {

    class ComboBox final : public WidgetWrapper {
        Q_OBJECT
        Q_PROPERTY(int count READ count)
        Q_PROPERTY(QVariant currentData READ currentData)
        Q_PROPERTY(QString currentText READ currentText)
        Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
    public:
        void render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) override;

        Q_INVOKABLE ComboBox();
        ~ComboBox() override;

        int count() const;
        QVariant currentData() const;
        QString currentText() const;
        int currentIndex() const;
        void setCurrentIndex(int index);

    public slots:
        void addItem(const QString &text, const QVariant &userData = {});
        void addItems(const QStringList &texts);
        void insertItem(int index, const QString &text, const QVariant &userData = {});
        void insertItems(int index, const QStringList &texts);
        QString itemText(int index) const;
        QVariant itemData(int index) const;
        void clear();

    signals:
        void currentIndexChanged(int index);

    private:
        inline QComboBox *comboBox() const {
            return qobject_cast<QComboBox *>(wrappedObject());
        }
    };

}

#endif //STIMMT_COMBOBOX_P_H
