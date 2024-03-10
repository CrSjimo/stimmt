//
// Created by Crs_1 on 2024/3/10.
//

#include "Button_p.h"

namespace stimmt::widgets {

    void AbstractButton::render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) {
        WidgetWrapper::render(wrappedObject, attributes, children, widgetsModule);
        if (children.property(0).isString()) {
            setText(children.property(0).toString());
        }
    }

    AbstractButton::AbstractButton(const QString &tagName, QAbstractButton *button) : WidgetWrapper(tagName, button) {
        connect(abstractButton(), &QAbstractButton::pressed, this, &AbstractButton::pressed);
        connect(abstractButton(), &QAbstractButton::released, this, &AbstractButton::released);
        connect(abstractButton(), &QAbstractButton::clicked, this, &AbstractButton::clicked);
        connect(abstractButton(), &QAbstractButton::toggled, this, &AbstractButton::toggled);
    }

    AbstractButton::~AbstractButton() = default;

    void AbstractButton::setText(const QString &text) {
        abstractButton()->setText(text);
    }

    QString AbstractButton::text() const {
        return abstractButton()->text();
    }

    void AbstractButton::setCheckable(bool a) {
        abstractButton()->setCheckable(a);
    }

    bool AbstractButton::isCheckable() const {
        return abstractButton()->isCheckable();
    }

    void AbstractButton::setChecked(bool a) {
        abstractButton()->setChecked(a);
    }

    bool AbstractButton::isChecked() const {
        return abstractButton()->isChecked();
    }

    void AbstractButton::setDown(bool a) {
        abstractButton()->setDown(a);
    }

    bool AbstractButton::isDown() const {
        return abstractButton()->isDown();
    }

    void AbstractButton::setAutoRepeat(bool a) {
        abstractButton()->setAutoRepeat(a);
    }

    bool AbstractButton::autoRepeat() const {
        return abstractButton()->autoRepeat();
    }

    void AbstractButton::setAutoRepeatDelay(int a) {
        abstractButton()->setAutoRepeatDelay(a);
    }

    int AbstractButton::autoRepeatDelay() const {
        return abstractButton()->autoRepeatDelay();
    }

    void AbstractButton::setAutoRepeatInterval(int a) {
        abstractButton()->setAutoRepeatInterval(a);
    }

    int AbstractButton::autoRepeatInterval() const {
        return abstractButton()->autoRepeatInterval();
    }

    void AbstractButton::setAutoExclusive(bool a) {
        abstractButton()->setAutoExclusive(a);
    }

    bool AbstractButton::autoExclusive() const {
        return abstractButton()->autoExclusive();
    }

    void AbstractButton::animateClick(int msec) {
        abstractButton()->animateClick(msec);
    }

    void AbstractButton::click() {
        abstractButton()->click();
    }

    void AbstractButton::toggle() {
        abstractButton()->toggle();
    }


    PushButton::PushButton() : AbstractButton("PushButton", new QPushButton) {
    }

    PushButton::~PushButton() = default;

    bool PushButton::autoDefault() const {
        return pushButton()->autoDefault();
    }

    void PushButton::setAutoDefault(bool a) {
        pushButton()->setAutoDefault(a);
    }

    bool PushButton::isDefault() const {
        return pushButton()->isDefault();
    }

    void PushButton::setDefault(bool a) {
        pushButton()->setDefault(a);
    }


    CheckBox::CheckBox() : AbstractButton("CheckBox", new QCheckBox) {
    }

    CheckBox::~CheckBox() = default;

    void CheckBox::setTristate(bool y) {
        checkBox()->setTristate(y);
    }

    bool CheckBox::isTristate() const {
        return false;
    }

    void CheckBox::setCheckState(CheckBox::CheckState state) {

    }

    CheckBox::CheckState CheckBox::checkState() const {
        return CheckBox::Checked;
    }

} // stimmt::widgets