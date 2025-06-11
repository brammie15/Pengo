#ifndef FOCUSABLE_H
#define FOCUSABLE_H
#include <unordered_map>
#include <vec2.hpp>

#include "ObjectModel/Component.h"


namespace fovy {
    enum class Direction;

    class Focusable: public Component {
    public:
        Focusable(GameObject& pParent, const std::string& name);
        void Update() override;

        void SetNavigationDirection(Direction dir, GameObject* target) {
            navigationOverrides[dir] = target;
        }

        [[nodiscard]] const std::unordered_map<Direction, GameObject*>& GetNavigationOverrides() const {
            return navigationOverrides;
        }


        virtual void OnSelect() = 0;
        virtual void OnDeselect() = 0;

        [[nodiscard]] virtual bool IsFocusable() const { return true; }
        virtual void OnInteract();

        virtual bool OnMove(Direction dir);

        virtual glm::vec2 GetSize() const {
            return {0.0f, 0.0f};
        }

    private:
        std::unordered_map<Direction, GameObject*> navigationOverrides;
    };
}


#endif //FOCUSABLE_H
