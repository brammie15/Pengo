#ifndef LETTERROLLER_H
#define LETTERROLLER_H
#include <vec2.hpp>

#include "Components/TextComponent.h"
#include "UI/Focusable.h"


namespace pengo {
    class LetterRoller: public fovy::Focusable{
    public:
        LetterRoller(fovy::GameObject& pParent);

        void Start() override;

        void Update() override;
        void Render() override;

        void OnSelect() override;
        void OnDeselect() override;

        glm::vec2 GetSize() const {
            return {75.0f, 100.0f};
        }

        bool OnMove(fovy::Direction dir) override;
        fovy::TextComponent* GetTextComponent() const {
            return m_textComponent;
        }

        [[nodiscard]] bool IsSelected() const { return m_selected; }

    private:
        bool m_selected{false};

        std::string m_currentLetter{"A"}; // Default letter
        fovy::TextComponent* m_textComponent{nullptr};
    };
}



#endif //LETTERROLLER_H
