#pragma once

struct Forces {
    float m_separationForce = 1.f;   // 3.f
    float m_cohesionForce   = 0.25f; // 0.25f
    float m_alignmentForce  = 1.f;   // 0.5f

    bool ImGui();
};