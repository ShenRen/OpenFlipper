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
    if (widget) {
        connect(widget, &QWidget::destroyed,
                this,   &QObject::deleteLater);
    }
}

QScriptValue createSettingsScriptObject(QScriptContext *ctx, QWidget *widget)
{
    QScriptEngine *engine = ctx->engine();
    ScriptSettings *obj = nullptr;
    if (auto w = dynamic_cast<BooleanWidget*>(widget)) {
        obj = new ScriptSettingsBoolean(w);
    }
    else if (auto w = dynamic_cast<DoubleWidget*>(widget)) {
        obj = new ScriptSettingsDouble(w);
    }
    else if (auto w = dynamic_cast<VectorWidget*>(widget)) {
        obj = new ScriptSettingsVector(w);
    }

    if (!obj) { // no ScriptSettings class implemented for this widget
        return QScriptValue::SpecialValue::NullValue;
    }
    return engine->newQObject(obj,
                              QScriptEngine::QtOwnership,
                              QScriptEngine::ExcludeSuperClassMethods
                              | QScriptEngine::ExcludeSuperClassProperties
                              | QScriptEngine::ExcludeChildObjects
                              );
}
