#include "forces.hpp"
#include "imgui.h"

bool Forces::ImGui()
{
    return ImGui::SliderFloat("Separation", &separationForce, 0.f, 5.f)
           || ImGui::SliderFloat("Cohesion", &cohesionForce, 0.f, 5.f)
           || ImGui::SliderFloat("Alignment", &alignmentForce, 0.f, 5.f);
}