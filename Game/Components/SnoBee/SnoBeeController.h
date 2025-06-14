#ifndef AI_CONTROLLER_H
#define AI_CONTROLLER_H

#include <memory>
#include <queue>

#include "Direction.h"
#include "SnoBeeComponent.h"
#include "Components/GridComponent.h"
#include "ObjectModel/Component.h"

namespace pengo {

    class SnoBeeComponent;

    class SnoBeeController final: public fovy::Component {
    public:
        explicit SnoBeeController(fovy::GameObject& parent, GridComponent* grid);
        ~SnoBeeController() override = default;

        void Update() override;
        void SetTarget(fovy::GameObject* target) { m_pTarget = target; }

    private:
        enum class AIState {
            Normal,
            Frenzy
        };

        class AICommand {
        public:
            virtual ~AICommand() = default;
            virtual void Execute(SnoBeeComponent* snoBee) = 0;
            virtual bool IsCompleted() const = 0;
        };

        class MoveCommand final: public AICommand {
        public:
            explicit MoveCommand(fovy::Direction direction) : m_direction(direction) {}

            void Execute(SnoBeeComponent* snoBee) override {
                snoBee->Move(m_direction);
            }

            [[nodiscard]] bool IsCompleted() const override {
                return true;
            }

        private:
            fovy::Direction m_direction;
        };

        class BreakCommand final : public AICommand {
        public:
            explicit BreakCommand(fovy::Direction direction) : m_direction(direction) {}

            void Execute(SnoBeeComponent* snoBee) override {
                snoBee->BreakIce(m_direction);
            }

            [[nodiscard]] bool IsCompleted() const override {
                return true;
            }

        private:
            fovy::Direction m_direction;
        };

        class IdleCommand : public AICommand {
        public:
            void Execute(SnoBeeComponent*) override {
            }

            bool IsCompleted() const override {
                return true;
            }
        };

        void UpdateAI();
        void EnterFrenzyMode();
        fovy::Direction CalculateNextMove();
        bool IsValidMove(const glm::ivec2& position, fovy::Direction direction) const;
        bool IsInCorridor(const glm::ivec2& position, fovy::Direction currentDir) const;
        int CountPossibleDirections(const glm::ivec2& position) const;
        bool IsStuck(const glm::ivec2& position) const;

        GridComponent* m_pGrid;
        fovy::GameObject* m_pTarget{nullptr};
        std::queue<std::unique_ptr<AICommand>> m_commandQueue;
        float m_decisionTimer{0.0f};
        float m_decisionInterval{1.0f};
        float m_rageTimer{0.0f};
        float m_rageTimeLimit{10.0f}; // Time before entering frenzy mode
        float m_frenzyDuration{5.0f}; // Duration of frenzy mode
        float m_frenzyTimer{0.0f};
        AIState m_currentState{AIState::Normal};
    };

}
#endif // AI_CONTROLLER_H