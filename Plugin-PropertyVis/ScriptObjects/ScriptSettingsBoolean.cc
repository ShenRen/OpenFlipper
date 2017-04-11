#include "ScriptSettingsBoolean.hh"
#include "ACG/Utils/ColorConversion.hh"

ScriptSettingsBoolean::ScriptSettingsBoolean()
    : ScriptSettings(nullptr),
      widget_(nullptr)
{
    assert(false);
}

ScriptSettingsBoolean::ScriptSettingsBoolean(BooleanWidget *widget)
    : ScriptSettings(widget),
      widget_(widget)
{}

ScriptSettingsBoolean::ScriptSettingsBoolean(const ScriptSettingsBoolean &other)
    : ScriptSettings(other.widget_),
      widget_(other.widget_)
{}

ScriptSettingsBoolean::~ScriptSettingsBoolean() {}

const Vector4 ScriptSettingsBoolean::colorTrue() const
{
    return ACG::to_Vec4d(widget_->colorTrue ->color());
}

const Vector4 ScriptSettingsBoolean::colorFalse() const
{
    return ACG::to_Vec4d(widget_->colorFalse->color());
}

void ScriptSettingsBoolean::setColorTrue(const Vector4 &color)
{
    widget_->colorTrue ->setColor(ACG::to_QColor(color));
}

void ScriptSettingsBoolean::setColorFalse(const Vector4 &color)
{
    widget_->colorFalse->setColor(ACG::to_QColor(color));
}

