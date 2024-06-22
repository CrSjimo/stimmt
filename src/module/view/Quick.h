#ifndef STIMMT_QUICK_H
#define STIMMT_QUICK_H

#include <QJSValue>

#include <stimmt/ViewContent.h>

namespace stimmt {
    class ModuleSystem;
}

namespace stimmt::modul {

    class QuickPrivate;

    class STIMMT_EXPORT Quick final : public ViewContent {
        Q_OBJECT
        Q_DECLARE_PRIVATE(Quick)
        Q_PROPERTY(ResizeMode resizeMode READ resizeMode WRITE setResizeMode)
    public:
        using ModuleSystemResolver = std::function<ModuleSystem *(QJSEngine *)>;

        Q_INVOKABLE Quick();
        ~Quick() override;

        QWidget *wrappedWidget() const override;

        enum ResizeMode { SizeViewToRootObject, SizeRootObjectToView };
        Q_ENUM(ResizeMode)
        void setResizeMode(ResizeMode mode);
        ResizeMode resizeMode() const;

        static void setModuleSystemResolver(const ModuleSystemResolver &resolver);

    public slots:
        void load(const QString &filePath);
        void sendMessage(const QString &name, const QJSValue &message);

    signals:
        void messageReceived(const QString &name, const QJSValue &message);

    private:
        QScopedPointer<QuickPrivate> d_ptr;
    };

}

#endif //STIMMT_QUICK_H
