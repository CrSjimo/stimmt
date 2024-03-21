#ifndef STIMMT_LINEEDIT_P_H
#define STIMMT_LINEEDIT_P_H

#include <QLineEdit>

#include <stimmt/WidgetWrapper.h>

namespace stimmt::widgets {

    class LineEdit final : public WidgetWrapper {
        Q_OBJECT
        Q_PROPERTY(QString text READ text WRITE setText)
        Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)
        Q_PROPERTY(int alignment READ alignment WRITE setAlignment)
    public:
        Q_INVOKABLE LineEdit();
        ~LineEdit() override;

        QString text() const;
        void setText(const QString &text);

        bool isReadOnly() const;
        void setReadOnly(bool readOnly);

        enum AlignmentFlag {
            AlignLeft = 0x0001,
            AlignLeading = AlignLeft,
            AlignRight = 0x0002,
            AlignTrailing = AlignRight,
            AlignHCenter = 0x0004,
            AlignJustify = 0x0008,
            AlignAbsolute = 0x0010,
            AlignHorizontal_Mask = AlignLeft | AlignRight | AlignHCenter | AlignJustify | AlignAbsolute,
            AlignTop = 0x0020,
            AlignBottom = 0x0040,
            AlignVCenter = 0x0080,
            AlignBaseline = 0x0100,
            AlignVertical_Mask = AlignTop | AlignBottom | AlignVCenter | AlignBaseline,
            AlignCenter = AlignVCenter | AlignHCenter
        };
        Q_ENUM(AlignmentFlag)
        int alignment() const;
        void setAlignment(int alignment);

    public slots:
        void clear();

    signals:
        void editingFinished();
        void returnPressed();
        void textChanged(const QString &text);
        void textEdited(const QString &text);

    private:
        inline QLineEdit *lineEdit() const {
            return qobject_cast<QLineEdit *>(wrappedObject());
        }
    };

}

#endif //STIMMT_LINEEDIT_P_H
