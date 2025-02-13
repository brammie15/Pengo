#pragma once
#include <glm.hpp>

namespace dae {
    class Transform final {
    public:
        [[nodiscard]] const glm::vec3 &GetPosition() const { return m_position; }

        void SetPosition(float x, float y, float z);
        const glm::vec3& GetPosition(){ return m_position; }

    private:
        glm::vec3 m_position{};
    };
}
