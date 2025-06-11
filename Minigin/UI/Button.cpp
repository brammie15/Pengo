#include "Button.h"

#include <iostream>

#include "Managers/Renderer.h"
#include "ObjectModel/GameObject.h"

fovy::Button::Button(GameObject& pParent, const std::string& text, std::shared_ptr<Font> font): Focusable(pParent, "Button"), m_Text(text), m_Font(std::move(font)) {
    if (!m_Font) {
        std::cerr << "Font not loaded for Button: " << text << std::endl;
    }

    //Kinda bad to do this here, but i need the text component to render the button
    m_TextComponent = this->GetGameObject()->AddComponent<TextComponent>(text, m_Font);
}

void fovy::Button::Update() {
}

void fovy::Button::Render() {
    //Draw box around
    Renderer::GetInstance().RenderRect(
        GetTransform().GetWorldPosition().x - 10,
        GetTransform().GetWorldPosition().y - 10,
        m_TextComponent->GetSize().x + 20,
        m_TextComponent->GetSize().y + 20,
        m_Selected ? SDL_Color(255,0,0,255) : SDL_Color(0, 0, 0, 255) // Red if selected, black otherwise
    );

    //TODO:
    //BAD BAD BAD i am rendering it twice now
    //Really need some way to sort the rendering order of components
    m_TextComponent->Render();
}

void fovy::Button::OnSelect() {
    std::cout << "Button selected: " << GetName() << std::endl;
    m_Selected = true;
}

void fovy::Button::OnDeselect() {
    std::cout << "Button deselected: " << GetName() << std::endl;
    m_Selected = false;
}

glm::vec2 fovy::Button::GetSize() const {
    return {
        m_TextComponent->GetSize().x + 20.0f, // Adding padding for the button
        m_TextComponent->GetSize().y + 20.0f
    };
}
