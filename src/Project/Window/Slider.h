#ifndef STIMMT_SLIDER_H
#define STIMMT_SLIDER_H

#include <QSlider>

#include <stimmt/ScriptObject.h>

namespace stimmt {
    class ProjectWindowObject;

    class Slider : public QSlider, public ScriptDescriptiveObject {
        Q_OBJECT
        Q_PROPERTY(bool hasTicks READ hasTicks WRITE setHasTicks)
    public:
        QJSValue createScriptObject() override;
        explicit Slider(QWidget *parent = nullptr);

        bool hasTicks() const;
        void setHasTicks(bool isTicksShown);
    };
}

#endif // STIMMT_SLIDER_H
