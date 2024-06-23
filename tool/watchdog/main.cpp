#include <QApplication>
#include <QSharedMemory>
#include <QTimer>
#include <QMessageBox>
#include <QStyle>
#include <QDebug>

enum WatchDogOperation {
    CloseNotification,
    OpenNotification,
    Interrupt,
};

struct Data {
    char operation;
    char flag;
};

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    qDebug() << "stimmt watchdog: starting";

    QSharedMemory sharedMemory(QApplication::arguments()[1]);
    if (!sharedMemory.attach())
        return 1;

    auto data = reinterpret_cast<Data *>(sharedMemory.data());

    QTimer timer;
    timer.setInterval(1000);
    timer.setSingleShot(false);
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setWindowTitle(QApplication::translate("stimmt::Watchdog", "Stimmt JavaScript Runtime"));
    msgBox.setText(QApplication::translate("stimmt::Watchdog", "JavaScript execution takes too long. Interrupt the execution?"));

    QObject::connect(&msgBox, &QDialog::accepted, [&] {
        data->operation = Interrupt;
    });

    QObject::connect(&msgBox, &QDialog::rejected, [&] {
        data->operation = CloseNotification;
    });

    char lastFlag = 0;
    int lastCount = 0;

    QObject::connect(&timer, &QTimer::timeout, [&] {
        if (data->flag == lastFlag) {
            lastCount++;
        } else {
            lastFlag = data->flag;
            lastCount = 0;
        }
        if (lastCount > 5)
            QApplication::quit();
        switch (data->operation) {
            case OpenNotification:
                if (!msgBox.isVisible()) {
                    qDebug() << "stimmt watchdog: time out";
                    msgBox.show();
                }
                break;
            default:
                msgBox.reject();
        }
    });

    timer.start();

    QApplication::setQuitOnLastWindowClosed(false);

    qDebug() << "stimmt watchdog: started";

    return a.exec();
}