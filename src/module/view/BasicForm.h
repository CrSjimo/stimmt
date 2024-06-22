#ifndef STIMMT_BASICFORM_H
#define STIMMT_BASICFORM_H

#include <stimmt/ViewContent.h>
#include <QJSValue>

namespace stimmt::modul {

    class STIMMT_EXPORT BasicFormRenderedWidget : public QObject {
        Q_OBJECT
    public:
        explicit BasicFormRenderedWidget(QObject *parent = nullptr);
        ~BasicFormRenderedWidget() override;

        virtual QWidget *widget() const = 0;
        virtual void setValue(const QJSValue &value) = 0;
        virtual QJSValue value() const = 0;

    signals:
        void valueChanged(const QJSValue &value);
    };

    class STIMMT_EXPORT BasicFormRenderer : public QObject {
        Q_OBJECT
    public:
        explicit BasicFormRenderer(QObject *parent = nullptr);
        ~BasicFormRenderer() override;

        virtual BasicFormRenderedWidget *render(const QJSValue &widgetData) = 0;
    };

    class BasicFormPrivate;

    class STIMMT_EXPORT BasicForm final : public ViewContent {
        Q_OBJECT
        Q_DECLARE_PRIVATE(BasicForm)
        Q_PROPERTY(QJSValue aboutToCompleteCallback READ aboutToCompleteCallback WRITE setAboutToCompleteCallback)
    public:
        Q_INVOKABLE BasicForm();
        ~BasicForm() override;

        QWidget *wrappedWidget() const override;

        void setAboutToCompleteCallback(const QJSValue &callback);
        QJSValue aboutToCompleteCallback() const;

        static void installRenderer(const QString &type, BasicFormRenderer *renderer);

    public slots:
        void addRow(const QString &id, const QString &label, const QString &type, const QJSValue &widget);

        QJSValue rowValue(const QString &id) const;
        void setRowValue(const QString &id, const QJSValue &value);

    signals:
        void rowValueChanged(const QString &id, const QJSValue &value);

    private:
        QScopedPointer<BasicFormPrivate> d_ptr;
    };

}

#endif //STIMMT_BASICFORM_H
