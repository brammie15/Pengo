#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <memory>
#include <SDL_pixels.h>
#include <string>

#include "ObjectModel/Component.h"
#include "Resources/Font.h"
#include "Resources/Texture2D.h"

namespace fovy {
    class TextComponent final: public Component {
    public:
        TextComponent(GameObject &parent, const std::string& text, std::shared_ptr<Font> font);

        void Render() override;
        void Update() override;
        void SetText(const std::string& newText);

        void ImGuiInspector() override;

        void SetFont(const std::shared_ptr<Font>& font) {
            m_Font = font;
            m_isDirty = true; // Mark as dirty to update the texture
        }

        [[nodiscard]] const std::string& GetText() const { return m_Text; }
        [[nodiscard]] const SDL_Color& GetColor() const { return m_Color; }

        TextComponent(const TextComponent& other) = delete;
        TextComponent(TextComponent&& other) noexcept = delete;
        TextComponent& operator=(const TextComponent& other) = delete;
        TextComponent& operator=(TextComponent&& other) noexcept = delete;

        ~TextComponent() override = default;

        [[nodiscard]] glm::vec2 GetSize() const {
            if (m_TextTexture) {
                return m_TextTexture->GetSize();
            }
            return {0.0f, 0.0f};
        }

        void SetColor(const SDL_Color& color) {
            m_Color = color;
            m_isDirty = true; // Mark as dirty to update the texture
        }

    private:
        bool m_isDirty{true};

        SDL_Color m_Color{255, 255, 255, 255};
        std::string m_Text{};
        std::shared_ptr<Texture2D> m_TextTexture{};
        std::shared_ptr<Font> m_Font;
    };
}
#endif //TEXTCOMPONENT_H
