#ifndef TILESTATES_H
#define TILESTATES_H
#include <memory>

#include "IceBlockComponent.h"
#include "../../../Minigin/Components/TextureComponent.h"

namespace pengo {
    class IceBlockState {
    public:
        virtual ~IceBlockState() = default;
        virtual void Enter(pengo::IceBlockComponent* iceBlock) = 0;
        virtual void Exit(pengo::IceBlockComponent* iceBlock) = 0;
        virtual std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) = 0;

        virtual std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock ) = 0;

        [[nodiscard]] std::string GetName() const {
            return typeid(*this).name();
        }
    };


    class IceBlockIdleState final: public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;
    };

    class IceBlockSlidingState final: public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;
    private:
        bool m_validMove{ false };
        glm::ivec2 m_slideDirection{};
        GridComponent* m_grid{ nullptr };
        IceBlockComponent* m_iceBlock{ nullptr };

        float m_slideTimer = 0.0f;
        float m_slideDuration = 0.15f;
        glm::vec2 m_startPos{};
        glm::vec2 m_targetPos{};
        bool m_updatedGrid = false;
    };

    class IceBlockBreakState final: public IceBlockState {
        void Enter(pengo::IceBlockComponent* iceBlock) override;
        void Exit(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* iceBlock) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* iceBlock) override;
    private:
        float m_breakDuration;
        float m_breakTimer = 0.0f;

    };

}


#endif //TILESTATES_H
