#ifndef SCRIPTSETTINGSVECTOR_HH
#define SCRIPTSETTINGSVECTOR_HH

#include <QObject>
#include "ScriptSettings.hh"

#include "Widgets/VectorWidget.hh"
#include "ACG/Math/VectorT.hh"
#include "OpenFlipper/common/DataTypes.hh"

class ScriptSettingsVector : public ScriptSettings
{
    Q_OBJECT

    //Q_PROPERTY(Vector4 colorTrue  READ colorTrue  WRITE setColorTrue )

public:
    explicit ScriptSettingsVector();
    explicit ScriptSettingsVector(VectorWidget *widget);
    explicit ScriptSettingsVector(const ScriptSettingsVector &other);
    virtual ~ScriptSettingsVector();
signals:

public slots:
private:
    VectorWidget *widget_;
};
Q_DECLARE_METATYPE(ScriptSettingsVector)

#endif // SCRIPTSETTINGSVECTOR_HH
