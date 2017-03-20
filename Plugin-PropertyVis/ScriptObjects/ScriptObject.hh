#ifndef SCRIPTOBJECT_HH
#define SCRIPTOBJECT_HH

#include <QScriptValue>

class QScriptContext;
class QWidget;

QScriptValue createSettingsScriptObject(QScriptContext *ctx, QWidget *widget);

#endif // SCRIPTOBJECT_HH
