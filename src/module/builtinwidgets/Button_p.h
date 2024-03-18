#ifndef STIMMT_BUTTON_P_H
#define STIMMT_BUTTON_P_H

#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>

#include <stimmt/WidgetWrapper.h>

namespace stimmt::widgets {

    class STIMMT_EXPORT AbstractButton : public WidgetWrapper {
        Q_OBJECT
        Q_PROPERTY(QString text READ text WRITE setText)
        Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)
        Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
        Q_PROPERTY(bool autoRepeat READ autoRepeat WRITE setAutoRepeat)
        Q_PROPERTY(bool autoExclusive READ autoExclusive WRITE setAutoExclusive)
        Q_PROPERTY(int autoRepeatDelay READ autoRepeatDelay WRITE setAutoRepeatDelay)
        Q_PROPERTY(int autoRepeatInterval READ autoRepeatInterval WRITE setAutoRepeatInterval)
        Q_PROPERTY(bool down READ isDown WRITE setDown)
    public:
        void render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) override;

        AbstractButton(const QString &tagName, QAbstractButton *button);
        ~AbstractButton() override;

        void setText(const QString &text);
        QString text() const;

        void setCheckable(bool);
        bool isCheckable() const;

        void setChecked(bool);
        bool isChecked() const;

        void setDown(bool);
        bool isDown() const;

        void setAutoRepeat(bool);
        bool autoRepeat() const;

        void setAutoRepeatDelay(int);
        int autoRepeatDelay() const;

        void setAutoRepeatInterval(int);
        int autoRepeatInterval() const;

        void setAutoExclusive(bool);
        bool autoExclusive() const;

    public slots:
        void animateClick(int msec = 100);
        void click();
        void toggle();

    signals:
        void pressed();
        void released();
        void clicked(bool checked = false);
        void toggled(bool checked);

    protected:
        inline QAbstractButton *abstractButton() const {
            return qobject_cast<QAbstractButton *>(wrappedObject());
        }
    };

    class PushButton final : public AbstractButton {
        Q_OBJECT
        Q_PROPERTY(bool autoDefault READ autoDefault WRITE setAutoDefault)
        Q_PROPERTY(bool default READ isDefault WRITE setDefault)
    public:
        Q_INVOKABLE PushButton();
        ~PushButton() override;

        bool autoDefault() const;
        void setAutoDefault(bool);
        bool isDefault() const;
        void setDefault(bool);

    private:
        inline QPushButton *pushButton() const {
            return qobject_cast<QPushButton *>(abstractButton());
        }
    };

    class CheckBox final : public AbstractButton {
        Q_OBJECT
        Q_PROPERTY(bool tristate READ isTristate WRITE setTristate)
        Q_PROPERTY(CheckState checkState READ checkState WRITE setCheckState)
    public:
        Q_INVOKABLE CheckBox();
        ~CheckBox() override;

        enum CheckState {
            Unchecked,
            PartiallyChecked,
            Checked
        };
        Q_ENUM(CheckState)

        void setTristate(bool y);
        bool isTristate() const;

        void setCheckState(CheckState state);
        CheckState checkState() const;

    private:
        inline QCheckBox *checkBox() const {
            return qobject_cast<QCheckBox *>(abstractButton());
        }
    };

    class RadioButton final : public AbstractButton {
        Q_OBJECT
    public:
        Q_INVOKABLE RadioButton();
        ~RadioButton() override;
    };

} // stimmt::widgets

#endif //STIMMT_BUTTON_P_H
