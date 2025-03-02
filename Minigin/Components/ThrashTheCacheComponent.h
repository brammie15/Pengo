#ifndef THRASHTHECACHECOMPONENT_H
#define THRASHTHECACHECOMPONENT_H
#include <vector>
#include <cmath>

#include "ObjectModel/Component.h"

struct transform {
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

struct GameObjectNormal {
    transform local{};
    int id{};
};

struct GameObjectPointer {
    transform* local{};
    int id{};
};

namespace dae {
    class ThrashTheCacheComponent final: public Component {
    public:
        explicit ThrashTheCacheComponent(GameObject& parent);

        ThrashTheCacheComponent(const ThrashTheCacheComponent& other) = delete;
        ThrashTheCacheComponent(ThrashTheCacheComponent&& other) noexcept = delete;
        ThrashTheCacheComponent& operator=(const ThrashTheCacheComponent& other) = delete;
        ThrashTheCacheComponent& operator=(ThrashTheCacheComponent&& other) noexcept = delete;

        void Update() override;

        void ImGuiRender() override;

        // void ImGuiInspector() override;
    private:
        int m_Exercise2SampleCount{10};
        int m_arraySize{static_cast<int>(std::pow(2, 26))};
        std::vector<std::vector<long long>> m_Exercise2Data{};
        std::vector<long long> m_Exercise2DataPlot{};

        int m_Exercise3SampleCount{10};
        std::vector<std::vector<long long>> m_Exercise3DataNormal{};
        std::vector<std::vector<long long>> m_Exercise3DataPointer{};
        std::vector<long long> m_Exercise3DataPlotNormal{};
        std::vector<long long> m_Exercise3DataPlotPointer{};
    };
}



#endif //THRASHTHECACHECOMPONENT_H
