#ifndef LETTERROLLER_H
#define LETTERROLLER_H
#include <vec2.hpp>

#include "Components/TextComponent.h"
#include "UI/Focusable.h"


namespace pengo {
    class LetterRoller final: public fovy::Focusable{
    public:
        explicit LetterRoller(fovy::GameObject& pParent);

        void Start() override;

        void Update() override;
        void Render() override;

        void OnSelect() override;
        void OnDeselect() override;

        [[nodiscard]] glm::vec2 GetSize() const {
            return {75.0f, 100.0f};
        }

        bool OnMove(fovy::Direction dir) override;
        [[nodiscard]] fovy::TextComponent* GetTextComponent() const {
            return m_textComponent;
        }

        [[nodiscard]] bool IsSelected() const { return m_selected; }

    private:
        bool m_selected{false};

        std::string m_currentLetter{"A"};
        fovy::TextComponent* m_textComponent{nullptr};

        float m_blinkTimer{0.0f};
        float m_blinkInterval{0.5f};

        bool m_blinking{ false };
    };
}



#endif //LETTERROLLER_H
