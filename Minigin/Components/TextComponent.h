#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <memory>
#include <SDL_pixels.h>
#include <string>

#include "Component.h"
#include "Font.h"
#include "Texture2D.h"
#include <memory>

namespace dae {
    class TextComponent final: public Component {
    public:
        TextComponent(GameObject* parent, std::string text, std::shared_ptr<Font> font);

        void Render() override;
        void Update() override;
        void SetText(const std::string& newText);

        TextComponent(const TextComponent& other) = delete;
        TextComponent(TextComponent&& other) noexcept = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other) noexcept = delete;

        ~TextComponent() override = default;

    private:
        bool m_isDirty{true};

        SDL_Color m_Color{255, 255, 255, 255};
        std::string m_Text{};
        std::shared_ptr<Texture2D> m_TextTexture{};
        std::shared_ptr<Font> m_Font;
    };
}
#endif //TEXTCOMPONENT_H
