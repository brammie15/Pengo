#ifndef BUTTON_H
#define BUTTON_H
#include <memory>

#include "Focusable.h"
#include "Components/TextComponent.h"
#include "Resources/Font.h"


namespace fovy {
    class Button final: public Focusable {
    public:
        Button(GameObject& pParent, const std::string& text, std::shared_ptr<Font> font);

        void Update() override;
        void Render() override;
        void OnSelect() override;
        void OnDeselect() override;

    private:
        std::string m_Text;
        std::shared_ptr<Font> m_Font;

        bool m_Selected{false};

        TextComponent* m_TextComponent{nullptr};
    };
}



#endif //BUTTON_H
