#ifndef BUTTON_H
#define BUTTON_H
#include <memory>

#include "Focusable.h"
#include "Components/TextComponent.h"
#include "Resources/Font.h"

#include <functional>

#include "Event.h"


namespace fovy {
    class Button final: public Focusable {
    public:
        Button(GameObject& pParent, const std::string& text, std::shared_ptr<Font> font);

        void Update() override;
        void Render() override;
        void OnSelect() override;
        void OnDeselect() override;

        void OnInteract() override {
            m_ClickEvent.Invoke();
        }

        [[nodiscard]] Event<>& GetClickEvent() {
            return m_ClickEvent;
        }

        glm::vec2 GetSize() const override;

        [[nodiscard]] bool IsFocusable() const override { return true; }

        [[nodiscard]] TextComponent* GetTextComponent() const { return m_TextComponent; }

    private:
        std::string m_Text;
        std::shared_ptr<Font> m_Font;

        bool m_Selected{false};

        TextComponent* m_TextComponent{nullptr};

        Event<> m_ClickEvent{};
    };
}



#endif //BUTTON_H
