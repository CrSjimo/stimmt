#include "ComboBox_p.h"

namespace stimmt::widgets {
    void ComboBox::render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) {
        for (int i = 0; i < children.property("length").toInt(); i++) {
            auto item = children.property(i);
            if (item.isString())
                addItem(children.property(i).toString());
            else if (item.property("tagName").toString() == "ComboBox-Item") {
                addItem(children.property(i).property("children").property(0).toString());
            }
        }
        WidgetWrapper::render(wrappedObject, attributes, children, widgetsModule);
    }

    ComboBox::ComboBox() : WidgetWrapper("ComboBox", new QComboBox) {
        connect(comboBox(), QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComboBox::currentIndexChanged);
    }

    ComboBox::~ComboBox() = default;

    int ComboBox::count() const {
        return comboBox()->count();
    }

    QVariant ComboBox::currentData() const {
        return comboBox()->currentData();
    }

    QString ComboBox::currentText() const {
        return comboBox()->currentText();
    }

    int ComboBox::currentIndex() const {
        return comboBox()->currentIndex();
    }

    void ComboBox::setCurrentIndex(int index) {
        comboBox()->setCurrentIndex(index);
    }

    void ComboBox::addItem(const QString &text, const QVariant &userData) {
        comboBox()->addItem(text, userData);
    }

    void ComboBox::addItems(const QStringList &texts) {
        comboBox()->addItems(texts);
    }

    void ComboBox::insertItem(int index, const QString &text, const QVariant &userData) {
        comboBox()->insertItem(index, text, userData);
    }

    void ComboBox::insertItems(int index, const QStringList &texts) {
        comboBox()->insertItems(index, texts);
    }

    QString ComboBox::itemText(int index) const {
        return comboBox()->itemText(index);
    }

    QVariant ComboBox::itemData(int index) const {
        return comboBox()->itemData(index);
    }

    void ComboBox::clear() {
        comboBox()->clear();
    }
}