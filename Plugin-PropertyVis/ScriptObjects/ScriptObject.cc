#include "ScriptObject.hh"

#include <QScriptContext>
#include <QScriptEngine>
#include <QWidget>

#include "Widgets/BooleanWidget.hh"

#include "ScriptSettingsBoolean.hh"
#include "ScriptSettingsDouble.hh"

QScriptValue createSettingsScriptObject(
        QScriptContext *ctx,
        QWidget *widget)
{
    QScriptEngine *engine = ctx->engine();
    auto bw = dynamic_cast<BooleanWidget*>(widget);
    if (bw) {return engine->newQObject(new ScriptSettingsBoolean(bw)); }
    auto dw = dynamic_cast<DoubleWidget*>(widget);
    if (dw) {return engine->newQObject(new ScriptSettingsDouble(dw)); }
    // not implemented:
    return QScriptValue::SpecialValue::NullValue;
}
