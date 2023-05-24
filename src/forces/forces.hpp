#pragma once

struct Forces {
    float separationForce = 1.f;   // 3.f
    float cohesionForce   = 0.25f; // 0.25f
    float alignmentForce  = 1.f;   // 0.5f

    bool ImGui();
};