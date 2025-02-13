#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include <memory>
#include <SDL_pixels.h>
#include <string>

#include "Component.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae {
    class TextComponent: public Component {
    public:
        TextComponent(GameObject* parent, std::string text, Font* font);

        void Render() override;
        void Update() override;
        void SetText(const std::string& newText);

        ~TextComponent() = default;
    private:

        bool m_isDirty{ true };

        SDL_Color m_Color{ 255, 255, 255, 255 };
        std::string m_Text{};
        Font* m_Font;
        std::shared_ptr<Texture2D> m_TextTexture{};
    };
}
#endif //TEXTCOMPONENT_H
