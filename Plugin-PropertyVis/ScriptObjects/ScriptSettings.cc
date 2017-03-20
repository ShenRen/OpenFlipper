#include "ScriptSettings.hh"

#include <QScriptContext>
#include <QScriptEngine>
#include <QWidget>

#include "Widgets/BooleanWidget.hh"
#include "Widgets/VectorWidget.hh"
#include "Widgets/DoubleWidget.hh"

#include "ScriptSettingsBoolean.hh"
#include "ScriptSettingsDouble.hh"
#include "ScriptSettingsVector.hh"


ScriptSettings::ScriptSettings(QWidget *widget)
{
    if (widget == nullptr)
        return;
    connect(widget, &QWidget::destroyed,
            this,   &QObject::deleteLater);
}

QScriptValue createSettingsScriptObject(QScriptContext *ctx, QWidget *widget)
{
    QScriptEngine *engine = ctx->engine();
    auto bw = dynamic_cast<BooleanWidget*>(widget);
    if (bw) {return engine->newQObject(new ScriptSettingsBoolean(bw)); }
    auto dw = dynamic_cast<DoubleWidget*>(widget);
    if (dw) {return engine->newQObject(new ScriptSettingsDouble(dw)); }
    auto dv = dynamic_cast<VectorWidget*>(widget);
    if (dv) {return engine->newQObject(new ScriptSettingsVector(dv)); }
    // not implemented:
    return QScriptValue::SpecialValue::NullValue;
}
