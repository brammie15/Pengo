#ifndef FOCUSABLE_H
#define FOCUSABLE_H
#include <unordered_map>

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

        [[nodiscard]] const std::unordered_map<Direction, GameObject*>& GetNavigationOverrides() const { return navigationOverrides; }

        virtual void OnSelect() = 0;
        virtual void OnDeselect() = 0;

        [[nodiscard]] bool IsFocusable() const { return true; }
        void OnInteract();

    private:
        std::unordered_map<Direction, GameObject*> navigationOverrides;
    };
}


#endif //FOCUSABLE_H
