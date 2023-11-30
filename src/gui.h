#pragma once

#include "simcoe/math/math.h"

#include "imgui/imgui.h"

namespace ImGui
{
    bool SliderDegrees(const char* label, math::Radians<float> *p_value, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char *format = "%.0f deg", ImGuiSliderFlags flags = 0);
    bool SliderDegrees(const char* label, math::Degrees<float> *p_value, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char *format = "%.0f deg", ImGuiSliderFlags flags = 0);
}
