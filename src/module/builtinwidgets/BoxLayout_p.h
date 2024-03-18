#ifndef STIMMT_BOXLAYOUT_P_H
#define STIMMT_BOXLAYOUT_P_H

#include <QBoxLayout>

#include <stimmt/WidgetWrapper.h>

namespace stimmt::widgets {

    class STIMMT_EXPORT BoxLayout : public WidgetWrapper {
        Q_OBJECT
        Q_PROPERTY(int count READ count)
    public:
        void render(const QJSValue &wrappedObject, const QJSValue &attributes, const QJSValue &children, const modul::Widgets *widgetsModule) override;
        QJSValue children() const override;

        BoxLayout(const QString &tagName, QBoxLayout *layout);
        ~BoxLayout() override;

        int count();

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

    public slots:
        void addWidget(const QJSValue &jsWidget, int stretch = 0, int alignment = Qt::Alignment());
        void addStretch(int stretch = 0);

        void insertWidget(int index, const QJSValue &jsWidget, int stretch = 0, int alignment = Qt::Alignment());
        void insertStretch(int index, int stretch = 0);

    protected:
        inline QBoxLayout *boxLayout() const {
            return qobject_cast<QBoxLayout *>(wrappedObject());
        }

    private:
        QJSValue m_children;
        QJSValue m_childrenInsertFunction;
    };

    class HBoxLayout final : public BoxLayout {
        Q_OBJECT
    public:
        Q_INVOKABLE HBoxLayout();
        ~HBoxLayout() override;
    };

    class VBoxLayout final : public BoxLayout {
        Q_OBJECT
    public:
        Q_INVOKABLE VBoxLayout();
        ~VBoxLayout() override;
    };

} // stimmt::widgets

#endif //STIMMT_BOXLAYOUT_P_H
