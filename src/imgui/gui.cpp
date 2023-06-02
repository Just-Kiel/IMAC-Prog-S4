#include "imgui/gui.hpp"
#include "imgui.h"

void ImguiControls()
{
    if (ImGui::BeginMenu("Help"))
    {
        ImGui::Text("Controls:");
        ImGui::Text("- Right click to add an obstacle");

        // Jump a line to separate the two texts not with empty string text
        ImGui::NewLine();

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

        ImGui::NewLine();

        ImGui::Text("With the help of:");
        ImGui::Text("- Enguerrand DE SMET");
        ImGui::Text("- Jules FOUCHY");

        ImGui::NewLine();

        ImGui::Text("Project made during the fourth semester at the IMAC engineering school.");
        ImGui::EndMenu();
    }
}

void ImguiInstancing(bool& instancing)
{
    if (ImGui::BeginMenu("Instancing"))
    {
        ImGui::Text("If this is checked, the boids and obstacles will be drawn with instancing and no LOD will be used.");

        ImGui::NewLine();

        // Checkbox for instancing
        ImGui::Checkbox("Instancing", &instancing);

        ImGui::EndMenu();
    }
}

void ImguiStartSimulationWindow(bool& startSimulation, float& speedBoids)
{
    if (startSimulation)
    {
        // Make a closable window to start the simulation
        ImGui::Begin("Paperplane and asteroids simulation", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // Center the window
        ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - ImGui::GetWindowSize().x / 2, ImGui::GetIO().DisplaySize.y / 2 - ImGui::GetWindowSize().y / 2));

        // Text to present the simulation
        // Center the text
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize("Welcome to the paperplane and asteroids simulation !").x / 2);
        ImGui::Text("Welcome to the paperplane and asteroids simulation !");

        ImGui::NewLine();

        ImGui::Text("This simulation is made with OpenGL and C++.");
        ImGui::Text("You can find the controls in the Help menu.");
        ImGui::Text("You can also find a lot of more parameters changeable and details in the top menu.");

        // Button to start the simulation
        // Center the button
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize("Start simulation").x / 2);
        if (ImGui::Button("Start simulation"))
        {
            speedBoids      = 1.0f;
            startSimulation = false;
        }

        ImGui::End();
    }
}