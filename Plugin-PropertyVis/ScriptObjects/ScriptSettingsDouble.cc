#include "ScriptSettingsDouble.hh"
#include "ACG/Utils/ColorConversion.hh"

ScriptSettingsDouble::ScriptSettingsDouble()
    : ScriptSettings(nullptr),
      widget_(nullptr)
{
    assert(false);
}

ScriptSettingsDouble::ScriptSettingsDouble(
        DoubleWidget *widget)
    : ScriptSettings(widget),
      widget_(widget)
{}

ScriptSettingsDouble::ScriptSettingsDouble(
            const ScriptSettingsDouble &other)
    : ScriptSettings(other.widget_),
      widget_(other.widget_)
{}

ScriptSettingsDouble::~ScriptSettingsDouble()
{}

double ScriptSettingsDouble::rangeMin() const
{
    return widget_->doubleFixedRangeMin->value();
}

double ScriptSettingsDouble::rangeMax() const
{
    return widget_->doubleFixedRangeMax->value();
}

const Vector4 ScriptSettingsDouble::colorMin() const
{
    return ACG::to_Vec4d( widget_->doubleMin->color());
}

const Vector4 ScriptSettingsDouble::colorMax() const
{
    return ACG::to_Vec4d( widget_->doubleMax->color());
}

void ScriptSettingsDouble::setColorMin(const Vector4 &color)
{
    widget_->doubleMin->setColor(ACG::to_QColor(color));
}

void ScriptSettingsDouble::setColorMax(const Vector4 &color)
{
    widget_->doubleMax->setColor(ACG::to_QColor(color));
}

bool ScriptSettingsDouble::useFixedRange() const
{
    return widget_->doubleFixedRange->isChecked();
}

bool ScriptSettingsDouble::mapOutsideRange() const
{
    return widget_->doubleMapOutsideRange->isChecked();
}

bool ScriptSettingsDouble::absolute() const
{
    return widget_->doubleAbsolute->isChecked();
}

void ScriptSettingsDouble::setUseColorCoder(bool useColorCoder)
{
    return widget_->doubleColorCoder->setChecked(useColorCoder);
}

void ScriptSettingsDouble::setUseFixedRange(bool useFixedRange)
{
    return widget_->doubleFixedRange->setChecked(useFixedRange);
}

void ScriptSettingsDouble::setRangeMin(double val)
{
    return widget_->doubleFixedRangeMin->setValue(val);
}

void ScriptSettingsDouble::setRangeMax(double val)
{
    return widget_->doubleFixedRangeMax->setValue(val);
}

void ScriptSettingsDouble::setMapOutsideRange(bool mapOutsideRange)
{
    return widget_->doubleMapOutsideRange->setChecked(mapOutsideRange);
}

void ScriptSettingsDouble::setAbsolute(bool absolute)
{
    return widget_->doubleAbsolute->setChecked(absolute);
}

bool ScriptSettingsDouble::useColorCoder() const
{
    return widget_->doubleColorCoder->isChecked();
}
