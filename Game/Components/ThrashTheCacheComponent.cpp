#include "ThrashTheCacheComponent.h"

#include <chrono>
#include <iostream>
#include <numeric>

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

fovy::ThrashTheCacheComponent::ThrashTheCacheComponent(GameObject& parent): Component(parent) {
}

void fovy::ThrashTheCacheComponent::Update() {
}

void fovy::ThrashTheCacheComponent::ImGuiRender() {
    ImGui::Begin("Exercise 2");
    ImGui::SetWindowSize(ImVec2{400, 500});
    ImGui::InputInt("##", &m_Exercise2SampleCount);
    ImGui::SameLine();
    ImGui::Text("# of samples");

    if (ImGui::Button("Thrash the cache")) {
        m_Exercise2Data.clear();
        m_Exercise2Data.resize(m_Exercise2SampleCount);
        int* data = new int[m_arraySize];
        for (int index{0}; index < m_arraySize; ++index) {
            data[index] = static_cast<int>(rand() * 10);
        }
        for (int currentSample = 0; currentSample < m_Exercise2SampleCount; ++currentSample) {
            m_Exercise2Data[currentSample] = std::vector<long long>{};
            for (int stepsize{1}; stepsize <= 1024; stepsize *= 2) {
                auto begin = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < m_arraySize; i += stepsize) {
                    data[i] *= 2;
                }
                auto end = std::chrono::high_resolution_clock::now();
                m_Exercise2Data[currentSample].push_back(
                    std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            }
        }
        delete[] data;

        m_Exercise2DataPlot.clear();
        int sum{0};
        for (int index{0}; index < 10; ++index) {
            sum = std::accumulate(m_Exercise2Data.begin(), m_Exercise2Data.end(), 0,
                                  [index] (int total, const std::vector<long long>& data) {
                                      return static_cast<int>(total + data[index]);
                                  });
            m_Exercise2DataPlot.push_back(sum / m_Exercise2SampleCount);
        }
    }

    if (!m_Exercise2Data.empty()) {
        ImPlot::BeginPlot("###", ImVec2{-1, 0}, ImPlotFlags_CanvasOnly);
        ImPlot::SetupAxis(ImAxis_X1, "Sample Size");
        ImPlot::SetupAxis(ImAxis_Y1, "Time (us)");
        ImPlot::PlotLine("####", m_Exercise2Data[0].data(), static_cast<int>(m_Exercise2Data[0].size()));
        ImPlot::EndPlot();
    }
    ImGui::End();


    ImGui::Begin("Exercise 3");
    ImGui::SetWindowSize(ImVec2{400, 500});
    ImGui::InputInt("##", &m_Exercise3SampleCount);
    ImGui::SameLine();
    ImGui::Text("# of samples");
    if (ImGui::Button("Thrash the cache with GameObject")) {
        m_Exercise3DataNormal.clear();
        m_Exercise3DataNormal.resize(m_Exercise3SampleCount);
        GameObjectNormal* data = new GameObjectNormal[m_arraySize];

        for (int currentSample = 0; currentSample < m_Exercise3SampleCount; ++currentSample) {
            m_Exercise3DataNormal[currentSample] = std::vector<long long>{};

            for (int stepsize{1}; stepsize <= 1024; stepsize *= 2) {
                auto begin = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < m_arraySize; i += stepsize) {
                    data[i].id *= 2;
                }
                auto end = std::chrono::high_resolution_clock::now();
                m_Exercise3DataNormal[currentSample].push_back(
                    std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            }
        }
        delete[] data;

        m_Exercise3DataPlotNormal.clear();
        int sum{0};
        for (int index{0}; index < 10; ++index) {
            sum = std::accumulate(m_Exercise3DataNormal.begin(), m_Exercise3DataNormal.end(), 0,
                                  [index] (int total, const std::vector<long long>& data) {
                                      return static_cast<int>(total + data[index]);
                                  });
            m_Exercise3DataPlotNormal.push_back(sum / m_Exercise3SampleCount);
        }
    }
    if (!m_Exercise3DataPlotNormal.empty()) {
        if (ImPlot::BeginPlot("###", ImVec2{-1, 0}, ImPlotFlags_CanvasOnly)) {
            ImPlot::SetupAxis(ImAxis_X1, "Sample Size");
            ImPlot::SetupAxis(ImAxis_Y1, "Time (us)");
            ImPlot::SetNextLineStyle(ImVec4{255, 0, 0, 255});
            ImPlot::PlotLine("####", m_Exercise3DataPlotNormal.data(),
                             static_cast<int>(m_Exercise3DataPlotNormal.size()));
            ImPlot::EndPlot();
        }
    }
    if (ImGui::Button("Thrash the cache with GameObjectAlt")) {
        m_Exercise3DataPointer.clear();
        m_Exercise3DataPointer.resize(m_Exercise3SampleCount);
        GameObjectPointer* data = new GameObjectPointer[m_arraySize];

        for (int currentSample = 0; currentSample < m_Exercise3SampleCount; ++currentSample) {
            m_Exercise3DataPointer[currentSample] = std::vector<long long>{};

            for (int stepsize{1}; stepsize <= 1024; stepsize *= 2) {
                auto begin = std::chrono::high_resolution_clock::now();
                for (int i = 0; i < m_arraySize; i += stepsize) {
                    data[i].id *= 2;
                }
                auto end = std::chrono::high_resolution_clock::now();
                m_Exercise3DataPointer[currentSample].push_back(
                    std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
            }
        }
        delete[] data;

        m_Exercise3DataPlotPointer.clear();
        int sum{0};
        for (int index{0}; index < 10; ++index) {
            sum = std::accumulate(m_Exercise3DataPointer.begin(), m_Exercise3DataPointer.end(), 0,
                                  [index] (int total, const std::vector<long long>& data) {
                                      return static_cast<int>(total + data[index]);
                                  });
            m_Exercise3DataPlotPointer.push_back(sum / m_Exercise3SampleCount);
        }
    }
    if (!m_Exercise3DataPlotPointer.empty()) {
        if (ImPlot::BeginPlot("#23", ImVec2{-1, 0}, ImPlotFlags_CanvasOnly)) {
            ImPlot::SetupAxis(ImAxis_X1, "Sample Size");
            ImPlot::SetupAxis(ImAxis_Y1, "Time (us)");
            ImPlot::PlotLine("####", m_Exercise3DataPlotPointer.data(),
                             static_cast<int>(m_Exercise3DataPlotPointer.size()));
            ImPlot::EndPlot();
        }
    }


    if (!m_Exercise3DataPlotNormal.empty() and !m_Exercise3DataPlotPointer.empty()) {
        if (ImPlot::BeginPlot("#13", ImVec2{-1, 0})) {
            ImPlot::SetupAxes("x","y");
            ImPlot::PlotLine("GameObjectAlt", m_Exercise3DataPlotPointer.data(),
                             static_cast<int>(m_Exercise3DataPlotPointer.size()));
            ImPlot::SetNextLineStyle(ImVec4{255, 0, 0, 255});
            ImPlot::PlotLine("GameObject", m_Exercise3DataPlotNormal.data(),
                             static_cast<int>(m_Exercise3DataPlotNormal.size()));
            ImPlot::EndPlot();
        }
    }
    ImGui::End();
}
