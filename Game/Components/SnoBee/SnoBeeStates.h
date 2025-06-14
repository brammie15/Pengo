#ifndef SNOWBEESTATES_H
#define SNOWBEESTATES_H

#include <memory>
#include <string>
#include <vec2.hpp>

#include "Direction.h"
#include "../../../Minigin/Components/SpriteRenderer.h"

namespace pengo {
    class SnoBeeComponent;

    class SnoBeeState {
    public:
        virtual ~SnoBeeState() = default;
        virtual void Enter(pengo::SnoBeeComponent* snoBee) = 0;
        virtual void Exit(pengo::SnoBeeComponent* snoBee) = 0;
        virtual std::unique_ptr<SnoBeeState> Update(pengo::SnoBeeComponent* snoBee) = 0;

        virtual std::unique_ptr<SnoBeeState> OnMove(pengo::SnoBeeComponent* snoBee, glm::ivec2 dir) = 0;
        virtual std::unique_ptr<SnoBeeState> OnPush(pengo::SnoBeeComponent* snoBee) = 0;

        virtual std::unique_ptr<SnoBeeState> OnBreak(pengo::SnoBeeComponent*) {
            return nullptr;
        };

        [[nodiscard]] virtual std::string GetName() const {
            return typeid(*this).name();
        }
    };

    class SnoBeeSpawnState final: public SnoBeeState {
    public:
        void Enter(pengo::SnoBeeComponent* snoBee) override;
        void Exit(pengo::SnoBeeComponent* snoBee) override;
        std::unique_ptr<SnoBeeState> Update(pengo::SnoBeeComponent* snoBee) override;

        std::unique_ptr<SnoBeeState> OnMove(pengo::SnoBeeComponent* snoBee, glm::ivec2 dir) override;
        std::unique_ptr<SnoBeeState> OnPush(pengo::SnoBeeComponent* snoBee) override;

    private:
        float m_spawnTimer{0.0f};
        float m_spawnDuration{1.0f};
    };

    class SnoBeeIdleState final: public SnoBeeState {
    public:
        void Enter(pengo::SnoBeeComponent* snoBee) override;
        void Exit(pengo::SnoBeeComponent* snoBee) override;
        std::unique_ptr<SnoBeeState> Update(pengo::SnoBeeComponent* snoBee) override;
        std::unique_ptr<SnoBeeState> OnMove(pengo::SnoBeeComponent* snoBee, glm::ivec2 dir) override;
        std::unique_ptr<SnoBeeState> OnPush(pengo::SnoBeeComponent* snoBee) override;

        std::unique_ptr<SnoBeeState> OnBreak(pengo::SnoBeeComponent* snoBee) override;
    };

    class SnoBeeMoveState final: public SnoBeeState {
    public:
        void Enter(pengo::SnoBeeComponent* snoBee) override;
        void Exit(pengo::SnoBeeComponent* snoBee) override;
        std::unique_ptr<SnoBeeState> Update(pengo::SnoBeeComponent* snoBee) override;
        std::unique_ptr<SnoBeeState> OnMove(pengo::SnoBeeComponent* snoBee, glm::ivec2 dir) override;
        std::unique_ptr<SnoBeeState> OnPush(pengo::SnoBeeComponent* snoBee) override;

    private:
        fovy::SpriteRenderer* m_spriteRenderer{nullptr};
        glm::vec2 m_StartPosition{};
        glm::vec2 m_TargetPosition{};
        float m_moveTimer{};
        float m_moveDuration{0.2f};
        bool m_updatedGrid{false};
        bool m_isValidMove{};
    };

    class SnobeeStunState final: public SnoBeeState {
    public:
        void Enter(pengo::SnoBeeComponent* snoBee) override;
        void Exit(pengo::SnoBeeComponent* snoBee) override;
        std::unique_ptr<SnoBeeState> Update(pengo::SnoBeeComponent* snoBee) override;
        std::unique_ptr<SnoBeeState> OnMove(pengo::SnoBeeComponent* snoBee, glm::ivec2 dir) override;
        std::unique_ptr<SnoBeeState> OnPush(pengo::SnoBeeComponent* snoBee) override;
    private:
        float m_stunTimer{0.0f};
        float m_stunDuration{};
    };
}


#endif //SNOWBEESTATES_H
