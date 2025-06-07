#ifndef PENGOSTATES_H
#define PENGOSTATES_H
#include <memory>

#include "Components/TextureComponent.h"

namespace pengo {
    class PengoComponent;

    class PengoState {
    public:
        virtual ~PengoState() = default;
        virtual void Enter(PengoComponent* comp) = 0;
        virtual void Exit(PengoComponent* comp) = 0;
        virtual std::unique_ptr<PengoState> Update(pengo::PengoComponent* pengo) = 0;

        virtual std::unique_ptr<PengoState> OnMove(pengo::PengoComponent* pengo, glm::ivec2 dir) = 0;
        virtual std::unique_ptr<PengoState> OnPush(pengo::PengoComponent* pengo) = 0;

        std::string GetName() const {
            return typeid(*this).name();
        }
    };


    class PengoMovingState final: public pengo::PengoState {
    public:
        void Enter(pengo::PengoComponent* comp) override;
        void Exit(pengo::PengoComponent* comp) override;
        std::unique_ptr<pengo::PengoState> Update(pengo::PengoComponent* pengo) override;

        std::unique_ptr<pengo::PengoState> OnMove(pengo::PengoComponent* pengo, glm::ivec2 dir) override;
        std::unique_ptr<PengoState> OnPush(pengo::PengoComponent* pengo) override;

    private:
        glm::vec2 m_TargetPosition{0.0f, 0.0f};
        glm::vec2 m_StartPosition{0.0f, 0.0f};

        bool m_isValidMove{false};

        float m_moveTimer{0.0f}; // Timer to handle movement duration
        float m_moveDuration{0.2f}; // Duration of movement in seconds

        bool m_updatedGrid{ false };
    };


    class PengoIdleState final: public PengoState {
    public:
        void Enter(PengoComponent* comp) override;
        void Exit(PengoComponent* comp) override;
        std::unique_ptr<PengoState> Update(pengo::PengoComponent* pengo) override;

        std::unique_ptr<PengoState> OnMove(pengo::PengoComponent*, glm::ivec2) override;
        std::unique_ptr<PengoState> OnPush(pengo::PengoComponent* pengo) override;

    private:
    };

    class PengoPushState final : public PengoState {
    public:
        void Enter(PengoComponent* comp) override;
        void Exit(PengoComponent* comp) override;
        std::unique_ptr<PengoState> Update(pengo::PengoComponent* pengo) override;


    };
}


#endif //PENGOSTATES_H
