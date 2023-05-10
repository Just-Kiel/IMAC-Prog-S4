#include "imgui/gui.hpp"
#include "imgui.h"

void ImguiControls()
{
    if (ImGui::BeginMenu("Help"))
    {
        ImGui::Text("Controls:");
        ImGui::Text("- Left click to add an obstacle");

        ImGui::Text("");

        ImGui::Text("- Drag the mouse to rotate the camera");
        ImGui::Text("- Use the arrows to move the camera or classics WASD (QZSD)");
        ImGui::Text("- Use SPACE to go up and SHIFT to go down with the camera");
        ImGui::EndMenu();
    }
}

void ImguiCredits()
{
    if (ImGui::BeginMenu("Credits"))
    {
        ImGui::Text("Made by:");
        ImGui::Text("- Olivia CREPIN");
        ImGui::Text("- Aurore LAFAURIE");

        ImGui::Text("");

        ImGui::Text("With the help of:");
        ImGui::Text("- Enguerrand DE SMET");
        ImGui::Text("- Jules FOUCHY");

        ImGui::Text("");

        ImGui::Text("Project made during the fourth semester at the IMAC engineering school.");
        ImGui::EndMenu();
    }
}