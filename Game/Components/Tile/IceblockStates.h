#ifndef TILESTATES_H
#define TILESTATES_H
#include <memory>

#include "../../../Minigin/Components/TextureComponent.h"
#include "Components/GridComponent.h"


namespace pengo {
    class IceBlockComponent;
    class IceBlockState {
    public:
        virtual ~IceBlockState() = default;
        virtual void Enter(pengo::IceBlockComponent* iceBlock) = 0;
        virtual void Exit(pengo::IceBlockComponent* iceBlock) = 0;
        virtual std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) = 0;

        virtual std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock ) = 0;

        virtual std::unique_ptr<IceBlockState> OnBreak(pengo ::IceBlockComponent*) { //For enemenies
            return nullptr;
        }

        [[nodiscard]] std::string GetName() const {
            return typeid(*this).name();
        }
    };

    class IceBlockSpawnState final : public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;

    private:
        float m_flashCounter = 0.0f;
        float m_flashDuration = 0.5f; // Duration for the flashing effect

        bool m_shouldFlash = false;
    };

    class IceBlockBreakState final: public IceBlockState {
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;
    private:
        float m_breakDuration{};
        float m_breakTimer{0.0f};

        bool m_isEgg{};

    };

    class IceBlockIdleState final: public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;

        std::unique_ptr<IceBlockState> OnBreak(pengo::IceBlockComponent*) override {
            return std::make_unique<IceBlockBreakState>();
        }
    };

    class IceBlockSlidingState final: public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;
    private:
        void PrepareSlide(const glm::ivec2& nextGrid);

        bool m_validMove{ false };
        glm::ivec2 m_slideDirection{};
        GridComponent* m_grid{ nullptr };
        IceBlockComponent* m_iceBlock{ nullptr };

        float m_slideTimer = 0.0f;
        float m_slideDuration = 0.15f;
        glm::vec2 m_startPos{};
        glm::vec2 m_targetPos{};
        bool m_updatedGrid = false;

        bool m_hasEnemy{ false };
        fovy::GameObject* m_attachedEnemy = nullptr;
    };

    class IceBlockEggBreakState final: public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;
    private:
        float m_breakTimer{ 0.0f };
        float m_breakDuration{0.0f}; // Duration for the egg break animation
    };



}


#endif //TILESTATES_H
