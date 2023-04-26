#include "perfs.hpp"
#include "imgui.h"

void Perfs::startTimer()
{
    m_start_before_counting = std::chrono::system_clock::now();
}

void Perfs::TimerComputeBoids()
{
    m_elapsed_compute_boids_seconds = std::chrono::system_clock::now() - m_start_before_counting;
}

void Perfs::TimerDrawBoids()
{
    m_elapsed_draw_boids_seconds = std::chrono::system_clock::now() - m_start_before_counting;
}

void Perfs::ImGui()
{
    ImGui::Text("Elapsed compute boids time: %fms", m_elapsed_compute_boids_seconds.count() * 1000.0);
    ImGui::Text("Elapsed draw boids time: %fms", m_elapsed_draw_boids_seconds.count() * 1000.0);
}