#include "ScriptSettingsVector.hh"
#include "ACG/Utils/ColorConversion.hh"

ScriptSettingsVector::ScriptSettingsVector()
    : ScriptSettings(nullptr),
      widget_(nullptr)
{
    assert(false);
}

ScriptSettingsVector::ScriptSettingsVector(
        VectorWidget *widget)
    : ScriptSettings(widget),
      widget_(widget)
{}

ScriptSettingsVector::ScriptSettingsVector(
            const ScriptSettingsVector &other)
    : ScriptSettingsVector(other.widget_)
{}

ScriptSettingsVector::~ScriptSettingsVector()
{}


ScriptSettingsVector::Type ScriptSettingsVector::type() const
{
    if (widget_->vectors_edges_rb->isChecked()) {
        return Type::Edges;
    }
    if (widget_->vectors_strokes_rb->isChecked()) {
        return Type::Strokes;
    }
    if (widget_->vectors_length_color_rb->isChecked()) {
        return Type::LengthColor;
    }
    if (widget_->vectors_colors_rb->isChecked()) {
        return Type::Rgb;
    }
    throw std::logic_error("No radiobutton selected?!");
}

void ScriptSettingsVector::setType(ScriptSettingsVector::Type type)
{
    switch(type)
    {
    case Type::Edges:
        widget_->vectors_edges_rb->setChecked(true);
        break;
    case Type::Strokes:
        widget_->vectors_strokes_rb->setChecked(true);
        break;
    case Type::LengthColor:
        widget_->vectors_length_color_rb->setChecked(true);
        break;
    case Type::Rgb:
        widget_->vectors_colors_rb->setChecked(true);
        break;
    }
}

bool ScriptSettingsVector::normalize() const
{
    return widget_->normalize->isChecked();
}
void ScriptSettingsVector::setNormalize(bool normalize)
{
    widget_->normalize->setChecked(normalize);
}

bool ScriptSettingsVector::doScale() const
{
    return widget_->scale ->isChecked();
}

void ScriptSettingsVector::setDoScale(bool doScale)
{
    widget_->scale ->setChecked(doScale);
}

double ScriptSettingsVector::scaleFactor() const
{
    return  widget_->scaleBox->value();
}

void ScriptSettingsVector::setScaleFactor(double scaleFactor)
{
    widget_->scaleBox->setValue(scaleFactor);
}

Vector4 ScriptSettingsVector::lineColor() const
{
    return ACG::to_Vec4d(widget_->lineColor->color());
}

void ScriptSettingsVector::setLineColor(Vector4 lineColor)
{
    widget_->lineColor->setColor(ACG::to_QColor(lineColor));
}
