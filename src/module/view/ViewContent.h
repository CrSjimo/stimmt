#ifndef STIMMT_VIEWCONTENT_H
#define STIMMT_VIEWCONTENT_H

#include <stimmt/stimmtGlobal.h>

#include <QObject>

namespace stimmt::modul {

    class STIMMT_EXPORT ViewContent : public QObject {
        Q_OBJECT
    public:
        ~ViewContent() override;
        virtual QWidget *wrappedWidget() const = 0;

    signals:
        void finished(int retCode);

    protected:
        explicit ViewContent(QObject *parent = nullptr);
    };

} // modul

#endif //STIMMT_VIEWCONTENT_H
