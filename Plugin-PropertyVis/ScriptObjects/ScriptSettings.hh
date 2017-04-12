#ifndef SCRIPTSETTINGS_HH
#define SCRIPTSETTINGS_HH

#include <QObject>
#include <QScriptValue>

class QWidget;
class QScriptContext;

class ScriptSettings : public QObject
{
    Q_OBJECT
public:
    explicit ScriptSettings(QWidget *widget);
};

QScriptValue createSettingsScriptObject(
        QScriptContext *ctx,
        QWidget *widget);



#endif // SCRIPTSETTINGS_HH
