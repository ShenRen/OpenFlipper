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

    Q_PROPERTY(Type    type         READ type         WRITE setType)
    Q_PROPERTY(bool    normalize    READ normalize    WRITE setNormalize)
    Q_PROPERTY(bool    doScale      READ doScale      WRITE setDoScale)
    Q_PROPERTY(double  scaleFactor  READ scaleFactor  WRITE setScaleFactor)
    Q_PROPERTY(Vector4 lineColor    READ lineColor    WRITE setLineColor)

public:
    enum class Type {Strokes, Rgb, Edges, LengthColor};
    Q_ENUM(Type)

    explicit ScriptSettingsVector();
    explicit ScriptSettingsVector(VectorWidget *widget);
    explicit ScriptSettingsVector(const ScriptSettingsVector &other);
    virtual ~ScriptSettingsVector();

    Type type() const;
    bool normalize() const;
    bool doScale() const;
    double scaleFactor() const;
    Vector4 lineColor() const;

public slots:
    void setType(Type type);
    void setNormalize(bool normalize);
    void setDoScale(bool doScale);
    void setScaleFactor(double scaleFactor);
    void setLineColor(Vector4 lineColor);

private:
    VectorWidget *widget_;
};
Q_DECLARE_METATYPE(ScriptSettingsVector)

#endif // SCRIPTSETTINGSVECTOR_HH
