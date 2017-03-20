#ifndef SCRIPTSETTINGSBOOLEAN_HH
#define SCRIPTSETTINGSBOOLEAN_HH

#include <QObject>

#include "Widgets/BooleanWidget.hh"
#include "ACG/Math/VectorT.hh"
#include "OpenFlipper/common/DataTypes.hh"

class ScriptSettingsBoolean : public QObject {
    Q_OBJECT
    // We use Vector4 instead of a proper color type as those are not (yet)
    // exposed to the scripting environment.
    Q_PROPERTY(Vector4 colorTrue  READ colorTrue  WRITE setColorTrue )
    Q_PROPERTY(Vector4 colorFalse READ colorFalse WRITE setColorFalse)
public:
    explicit ScriptSettingsBoolean();
    explicit ScriptSettingsBoolean(BooleanWidget *widget);
    explicit ScriptSettingsBoolean(const ScriptSettingsBoolean &other);
    virtual ~ScriptSettingsBoolean();

    const Vector4 colorTrue()  const;
    const Vector4 colorFalse() const;
public slots:
    void setColorTrue (const Vector4 &color);
    void setColorFalse(const Vector4 &color);

private:
    BooleanWidget *widget_;
};
Q_DECLARE_METATYPE(ScriptSettingsBoolean)


#endif // SCRIPTSETTINGSBOOLEAN_HH
