#include "ScriptSettingsVector.hh"

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
