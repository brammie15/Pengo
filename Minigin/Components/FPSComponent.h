//
// Created by Bram on 12/02/2025.
//

#ifndef FPSCOMPONENT_H
#define FPSCOMPONENT_H
#include "Component.h"
#include "TextComponent.h"

namespace dae {

class FPSComponent final: public Component {
public:
    explicit FPSComponent(GameObject* parent);

    void Update() override;

    void Render() override;

    void SetTargetTextComponent(TextComponent* component);
private:
    float m_fps{0.0f};

    TextComponent* m_textComponent{ nullptr };
};

} // dae

#endif //FPSCOMPONENT_H
