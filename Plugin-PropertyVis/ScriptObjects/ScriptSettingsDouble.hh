#ifndef SCRIPTSETTINGSDOUBLE_HH
#define SCRIPTSETTINGSDOUBLE_HH

#include <QObject>
#include "ScriptSettings.hh"

#include "Widgets/DoubleWidget.hh"
#include "ACG/Math/VectorT.hh"
#include "OpenFlipper/common/DataTypes.hh"

class ScriptSettingsDouble : public ScriptSettings {
    Q_OBJECT
    // We use Vector4 instead of a proper color type as those are not (yet)
    // exposed to the scripting environment.
    Q_PROPERTY(bool useColorCoder READ useColorCoder WRITE setUseColorCoder)
    Q_PROPERTY(Vector4 colorMin READ colorMin WRITE setColorMin)
    Q_PROPERTY(Vector4 colorMax READ colorMax WRITE setColorMax)

    Q_PROPERTY(bool useFixedRange READ useFixedRange WRITE setUseFixedRange)
    Q_PROPERTY(double rangeMin READ rangeMin WRITE setRangeMin)
    Q_PROPERTY(double rangeMax READ rangeMax WRITE setRangeMax)

    Q_PROPERTY(bool mapOutsideRange READ mapOutsideRange WRITE setMapOutsideRange)
    Q_PROPERTY(bool absolute READ absolute WRITE setAbsolute)

public:
    explicit ScriptSettingsDouble();
    explicit ScriptSettingsDouble(DoubleWidget *widget);
    explicit ScriptSettingsDouble(const ScriptSettingsDouble &other);
    virtual ~ScriptSettingsDouble();

    bool useColorCoder() const;
    const Vector4 colorMin()  const;
    const Vector4 colorMax() const;
    bool useFixedRange() const;
    double rangeMin() const;
    double rangeMax() const;
    bool mapOutsideRange() const;
    bool absolute() const;

public slots:
    void setUseColorCoder(bool useColorCoder);
    void setColorMin(const Vector4 &color);
    void setColorMax(const Vector4 &color);
    void setUseFixedRange(bool useFixedRange);
    void setRangeMin(double val);
    void setRangeMax(double val);
    void setMapOutsideRange(bool mapOutsideRange);
    void setAbsolute(bool absolute);

private:
    DoubleWidget *widget_;
};
Q_DECLARE_METATYPE(ScriptSettingsDouble)

#endif // SCRIPTSETTINGSDOUBLE_HH
