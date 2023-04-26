#pragma once
#include <chrono>

struct Perfs {
    std::chrono::duration<double> m_elapsed_compute_boids_seconds{};
    std::chrono::duration<double> m_elapsed_draw_boids_seconds{};

    std::chrono::system_clock::time_point m_start_before_counting{};

    void startTimer();
    void TimerComputeBoids();
    void TimerDrawBoids();

    void ImGui();
};