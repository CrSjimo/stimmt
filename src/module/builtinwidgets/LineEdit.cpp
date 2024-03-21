#include "LineEdit_p.h"

namespace stimmt::widgets {
    LineEdit::LineEdit() : WidgetWrapper("LineEdit", new QLineEdit) {
        connect(lineEdit(), &QLineEdit::editingFinished, this, &LineEdit::editingFinished);
        connect(lineEdit(), &QLineEdit::returnPressed, this, &LineEdit::returnPressed);
        connect(lineEdit(), &QLineEdit::textChanged, this, &LineEdit::textChanged);
        connect(lineEdit(), &QLineEdit::textEdited, this, &LineEdit::textEdited);
    }

    LineEdit::~LineEdit() = default;

    QString LineEdit::text() const {
        return lineEdit()->text();
    }

    void LineEdit::setText(const QString &text) {
        lineEdit()->setText(text);
    }

    bool LineEdit::isReadOnly() const {
        return lineEdit()->isReadOnly();
    }

    void LineEdit::setReadOnly(bool readOnly) {
        lineEdit()->setReadOnly(readOnly);
    }

    int LineEdit::alignment() const {
        return lineEdit()->alignment();
    }

    void LineEdit::setAlignment(int alignment) {
        lineEdit()->setAlignment(Qt::Alignment(alignment));
    }

    void LineEdit::clear() {
        lineEdit()->clear();
    }
}