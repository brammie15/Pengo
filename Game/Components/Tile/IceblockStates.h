#ifndef TILESTATES_H
#define TILESTATES_H
#include <memory>

#include "IceBlockComponent.h"
#include "Components/TextureComponent.h"

namespace pengo {
    class IceBlockState {
    public:
        virtual ~IceBlockState() = default;
        virtual void Enter(pengo::IceBlockComponent* comp) = 0;
        virtual void Exit(pengo::IceBlockComponent* comp) = 0;
        virtual std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* pengo) = 0;

        virtual std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* pengo) = 0;

        std::string GetName() const {
            return typeid(*this).name();
        }
    };


    class IceBlockIdleState: public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* comp) override;
        void Exit(pengo::IceBlockComponent* comp) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* pengo) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* pengo) override;
    };

    class IceBlockSlidingState: public IceBlockState {
    public:
        void Enter(pengo::IceBlockComponent* comp) override;
        void Exit(pengo::IceBlockComponent* comp) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* pengo) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* pengo) override;
    private:
        bool m_validMove{ false };
    };

    class IceBlockBreakState: public IceBlockState {
        void Enter(pengo::IceBlockComponent* comp) override;
        void Exit(pengo::IceBlockComponent* comp) override;
        std::unique_ptr<IceBlockState> Update(pengo::IceBlockComponent* pengo) override;
        std::unique_ptr<IceBlockState> OnPush(pengo::IceBlockComponent* pengo) override;
    };

}


#endif //TILESTATES_H
