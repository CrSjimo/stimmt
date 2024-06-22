#include "BasicForm.h"
#include "BasicForm_p.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>
#include <QJSEngine>
#include <QFormLayout>
#include <QPushButton>


namespace stimmt::modul {

    BasicFormRenderedWidget::BasicFormRenderedWidget(QObject *parent) : QObject(parent) {
    }

    BasicFormRenderedWidget::~BasicFormRenderedWidget() = default;

    BasicFormRenderer::BasicFormRenderer(QObject *parent) : QObject(parent) {
    }

    BasicFormRenderer::~BasicFormRenderer() = default;

    static QHash<QString, BasicFormRenderer *> m_renderers;

    BasicForm::BasicForm() : d_ptr(new BasicFormPrivate) {
        Q_D(BasicForm);
        d->q_ptr = this;
        d->widget = new QWidget;
        auto mainLayout = new QVBoxLayout;
        d->formLayout = new QFormLayout;
        mainLayout->addLayout(d->formLayout);
        auto buttonLayout = new QHBoxLayout;
        buttonLayout->addStretch();
        auto okButton = new QPushButton(tr("OK"));
        buttonLayout->addWidget(okButton);
        auto cancelButton = new QPushButton(tr("Cancel"));
        buttonLayout->addWidget(cancelButton);
        mainLayout->addLayout(buttonLayout);
        d->widget->setLayout(mainLayout);

        connect(okButton, &QAbstractButton::clicked, this, [=] {
            if (d->aboutToCompleteCallback.isCallable()) {
                if (d->aboutToCompleteCallback.call().toBool())
                    emit finished(0);
            } else {
                emit finished(0);
            }
        });

        connect(cancelButton, &QAbstractButton::clicked, this, [=] {
            emit finished(1);
        });
    }

    BasicForm::~BasicForm() {
        Q_D(BasicForm);
        delete d->widget.data();
    }

    QWidget *BasicForm::wrappedWidget() const {
        Q_D(const BasicForm);
        return d->widget;
    }

    void BasicForm::setAboutToCompleteCallback(const QJSValue &callback) {
        Q_D(BasicForm);
        d->aboutToCompleteCallback = callback;
    }

    QJSValue BasicForm::aboutToCompleteCallback() const {
        Q_D(const BasicForm);
        return d->aboutToCompleteCallback;
    }

    void BasicForm::addRow(const QString &id, const QString &label, const QString &type, const QJSValue &widget) {
        Q_D(BasicForm);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (d->rows.contains(id)) {
            engine->throwError(QStringLiteral("Row ID '%1' already exists").arg(id));
            return;
        }
        auto renderer = m_renderers.value(type);
        if (!renderer) {
            engine->throwError(QStringLiteral("Unknown type '%1'").arg(type));
            return;
        }
        auto renderedWidget = renderer->render(widget);
        renderedWidget->setParent(this);
        d->formLayout->addRow(label, renderedWidget->widget());
        connect(renderedWidget, &BasicFormRenderedWidget::valueChanged, this, [=](const QJSValue &value) {
            emit rowValueChanged(id, value);
        });
        d->rows.insert(id, renderedWidget);
    }

    QJSValue BasicForm::rowValue(const QString &id) const {
        Q_D(const BasicForm);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!d->rows.contains(id)) {
            engine->throwError(QStringLiteral("Row ID '%1' does not exist").arg(id));
            return {};
        }
        return d->rows.value(id)->value();
    }

    void BasicForm::setRowValue(const QString &id, const QJSValue &value) {
        Q_D(BasicForm);
        auto engine = qjsEngine(this);
        Q_ASSERT(engine);
        if (!d->rows.contains(id)) {
            engine->throwError(QStringLiteral("Row ID '%1' does not exist").arg(id));
            return;
        }
        d->rows.value(id)->setValue(value);
    }

#define DECLARE_RENDERED_WIDGET2(ClassName, WidgetClass, GetMethod, SetMethod, ConvertMethod, Signal, Slot) \
class ClassName : public BasicFormRenderedWidget { \
    Q_OBJECT \
public: \
    explicit ClassName(QObject *parent = nullptr) : BasicFormRenderedWidget(parent) { \
        m_widget = new WidgetClass; \
        connect(m_widget, Signal, this, Slot); \
    } \
    ~ClassName() override = default; \
    QWidget *widget() const override { \
        return m_widget; \
    } \
    void setValue(const QJSValue &value) override { \
        m_widget->SetMethod(value.ConvertMethod()); \
    } \
    QJSValue value() const override { \
        return m_widget->GetMethod(); \
    } \
    WidgetClass *m_widget; \
};

#define DECLARE_RENDERED_WIDGET(ClassName, WidgetClass, GetMethod, SetMethod, ConvertMethod, Signal) DECLARE_RENDERED_WIDGET2(ClassName, WidgetClass, GetMethod, SetMethod, ConvertMethod, Signal, &BasicFormRenderedWidget::valueChanged)

    DECLARE_RENDERED_WIDGET(TextBoxRenderedWidget, QLineEdit, text, setText, toString, &QLineEdit::textChanged)
    class TextBoxRenderer : public BasicFormRenderer {
    public:
        explicit TextBoxRenderer(QObject *parent = nullptr) : BasicFormRenderer(parent) {
        }
        ~TextBoxRenderer() override = default;

        BasicFormRenderedWidget *render(const QJSValue &widgetData) override {
            auto renderedWidget = new TextBoxRenderedWidget;
            renderedWidget->m_widget->setPlaceholderText(widgetData.hasProperty("placeholder") ? widgetData.property("placeholder").toString() : QString());
            renderedWidget->m_widget->setText(widgetData.hasProperty("value") ? widgetData.property("value").toString() : QString());
            return renderedWidget;
        }
    };

    DECLARE_RENDERED_WIDGET2(TextAreaRenderedWidget, QPlainTextEdit, toPlainText, setPlainText, toString, &QPlainTextEdit::textChanged, [=](){ emit valueChanged(m_widget->toPlainText()); })
    class TextAreaRenderer : public BasicFormRenderer {
    public:
        explicit TextAreaRenderer(QObject *parent = nullptr) : BasicFormRenderer(parent) {
        }
        ~TextAreaRenderer() override = default;

        BasicFormRenderedWidget *render(const QJSValue &widgetData) override {
            auto renderedWidget = new TextAreaRenderedWidget;
            renderedWidget->m_widget->setPlaceholderText(widgetData.hasProperty("placeholder") ? widgetData.property("placeholder").toString() : QString());
            renderedWidget->m_widget->setPlainText(widgetData.hasProperty("value") ? widgetData.property("value").toString() : QString());
            return renderedWidget;
        }
    };

    DECLARE_RENDERED_WIDGET(ComboBoxRenderedWidget, QComboBox, currentIndex, setCurrentIndex, toInt, QOverload<int>::of(&QComboBox::currentIndexChanged))
    class ComboBoxRenderer : public BasicFormRenderer {
    public:
        explicit ComboBoxRenderer(QObject *parent = nullptr) : BasicFormRenderer(parent) {
        }
        ~ComboBoxRenderer() override = default;

        BasicFormRenderedWidget *render(const QJSValue &widgetData) override {
            auto renderedWidget = new ComboBoxRenderedWidget;
            auto itemArray = widgetData.property("items");
            for (int i = 0; i < itemArray.property("length").toInt(); i++)
                renderedWidget->m_widget->addItem(itemArray.property(i).toString());
            renderedWidget->m_widget->setCurrentIndex(widgetData.property("value").toInt());
            return renderedWidget;
        }
    };

    DECLARE_RENDERED_WIDGET(CheckBoxRenderedWidget, QCheckBox, isChecked, setChecked, toBool, &QAbstractButton::toggled)
    class CheckBoxRenderer : public BasicFormRenderer {
    public:
        explicit CheckBoxRenderer(QObject *parent = nullptr) : BasicFormRenderer(parent) {
        }
        ~CheckBoxRenderer() override = default;

        BasicFormRenderedWidget *render(const QJSValue &widgetData) override {
            auto renderedWidget = new CheckBoxRenderedWidget;
            renderedWidget->m_widget->setChecked(widgetData.property("value").toBool());
            return renderedWidget;
        }
    };

    DECLARE_RENDERED_WIDGET(SliderRenderedWidget, QSlider, value, setValue, toInt, &QAbstractSlider::valueChanged)
    class SliderRenderer : public BasicFormRenderer {
    public:
        explicit SliderRenderer(QObject *parent = nullptr) : BasicFormRenderer(parent) {
        }
        ~SliderRenderer() override = default;

        BasicFormRenderedWidget *render(const QJSValue &widgetData) override {
            auto renderedWidget = new SliderRenderedWidget;
            renderedWidget->m_widget->setMinimum(widgetData.hasProperty("minimum") ? widgetData.property("minimum").toInt() : std::numeric_limits<int>::min());
            renderedWidget->m_widget->setMaximum(widgetData.hasProperty("maximum") ? widgetData.property("maximum").toInt() : std::numeric_limits<int>::max());
            renderedWidget->m_widget->setValue(widgetData.property("value").toInt());
            return renderedWidget;
        }
    };

    DECLARE_RENDERED_WIDGET(SpinBoxRenderedWidget, QSpinBox, value, setValue, toInt, QOverload<int>::of(&QSpinBox::valueChanged))
    class SpinBoxRenderer : public BasicFormRenderer {
    public:
        explicit SpinBoxRenderer(QObject *parent = nullptr) : BasicFormRenderer(parent) {
        }
        ~SpinBoxRenderer() override = default;

        BasicFormRenderedWidget *render(const QJSValue &widgetData) override {
            auto renderedWidget = new SpinBoxRenderedWidget;
            renderedWidget->m_widget->setMinimum(widgetData.hasProperty("minimum") ? widgetData.property("minimum").toInt() : std::numeric_limits<int>::min());
            renderedWidget->m_widget->setMaximum(widgetData.hasProperty("maximum") ? widgetData.property("maximum").toInt() : std::numeric_limits<int>::max());
            renderedWidget->m_widget->setValue(widgetData.property("value").toInt());
            return renderedWidget;
        }
    };

    DECLARE_RENDERED_WIDGET(DoubleSpinBoxRenderedWidget, QDoubleSpinBox, value, setValue, toInt, QOverload<double>::of(&QDoubleSpinBox::valueChanged))
    class DoubleSpinBoxRenderer : public BasicFormRenderer {
    public:
        explicit DoubleSpinBoxRenderer(QObject *parent = nullptr) : BasicFormRenderer(parent) {
        }
        ~DoubleSpinBoxRenderer() override = default;

        BasicFormRenderedWidget *render(const QJSValue &widgetData) override {
            auto renderedWidget = new DoubleSpinBoxRenderedWidget;
            renderedWidget->m_widget->setMinimum(widgetData.hasProperty("minimum") ? widgetData.property("minimum").toNumber() : std::numeric_limits<double>::min());
            renderedWidget->m_widget->setMaximum(widgetData.hasProperty("maximum") ? widgetData.property("maximum").toNumber() : std::numeric_limits<double>::max());
            renderedWidget->m_widget->setDecimals(widgetData.hasProperty("decimals") ? widgetData.property("decimals").toInt() : 2);
            renderedWidget->m_widget->setValue(widgetData.property("value").toInt());
            return renderedWidget;
        }
    };

    namespace {
        struct A {
            A() {
                BasicForm::installRenderer("TextBox", new TextBoxRenderer);
                BasicForm::installRenderer("TextArea", new TextAreaRenderer);
                BasicForm::installRenderer("ComboBox", new ComboBoxRenderer);
                BasicForm::installRenderer("CheckBox", new CheckBoxRenderer);
                BasicForm::installRenderer("Slider", new SliderRenderer);
                BasicForm::installRenderer("SpinBox", new SpinBoxRenderer);
                BasicForm::installRenderer("DoubleSpinBox", new DoubleSpinBoxRenderer);
            }
        } a;
    }

    void BasicForm::installRenderer(const QString &type, BasicFormRenderer *renderer) {
        static QObject o;
        if (!m_renderers.contains(type)) {
            renderer->setParent(&o);
            m_renderers.insert(type, renderer);
            connect(renderer, &QObject::destroyed, [=] {
                m_renderers.remove(type);
            });
        }
    }

} // stimmt

#include "BasicForm.moc"