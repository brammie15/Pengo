#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H

#include "../../Minigin/Components/TextComponent.h"
#include "ObjectModel/Component.h"

namespace fovy {
    class FPSComponent final: public Component {
    public:
        explicit FPSComponent(GameObject &parent);

        void Update() override;

        void Render() override;

        void SetTargetTextComponent(TextComponent* component);

    private:
        float m_fps{0.0f};

        TextComponent* m_textComponent{nullptr};
    };
} // dae

#endif //FPSCOMPONENT_H
