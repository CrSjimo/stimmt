#ifndef STIMMT_CODEC_H
#define STIMMT_CODEC_H

#include <QObject>

#include <stimmt/StimmtGlobal.h>

class QJSEngine;

namespace stimmt::extension {

    class STIMMT_EXPORT Codec : public QObject {
        Q_OBJECT
    public:
        explicit Codec(QJSEngine *engine, QObject *parent = nullptr);
        ~Codec() override;
    };

} // stimmt::extension

#endif //STIMMT_CODEC_H
