#include "gui.h"

bool ImGui::SliderDegrees(const char* label, math::Radians<float> *p_value, float v_degrees_min, float v_degrees_max, const char *format, ImGuiSliderFlags flags)
{
    float v_rad = p_value->radians();
    bool ret = ImGui::SliderAngle(label, &v_rad, v_degrees_min, v_degrees_max, format, flags);

    if (ret) *p_value = math::Radians<float>(v_rad);

    return ret;
}

bool ImGui::SliderDegrees(const char* label, math::Degrees<float> *p_value, float v_degrees_min, float v_degrees_max, const char *format, ImGuiSliderFlags flags)
{
    float v_rad = p_value->radians();
    bool ret = ImGui::SliderAngle(label, &v_rad, v_degrees_min, v_degrees_max, format, flags);

    if (ret) *p_value = math::Radians<float>(v_rad);

    return ret;
}
