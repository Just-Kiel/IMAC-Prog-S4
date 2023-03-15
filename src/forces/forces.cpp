#include "forces.hpp"
#include "imgui.h"

bool Forces::ImGui()
{
    return ImGui::SliderFloat("Separation", &m_separationForce, 0.f, 5.f)
           || ImGui::SliderFloat("Cohesion", &m_cohesionForce, 0.f, 5.f)
           || ImGui::SliderFloat("Alignment", &m_alignmentForce, 0.f, 5.f);
}