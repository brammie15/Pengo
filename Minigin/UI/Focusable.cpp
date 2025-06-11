#include "Focusable.h"

#include <iostream>

fovy::Focusable::Focusable(GameObject& pParent, const std::string& name): Component{pParent, name} {
}

void fovy::Focusable::Update() {
}

void fovy::Focusable::OnInteract() {
    std::cout << "Focusable interacted with: " << GetName() << std::endl;
}

bool fovy::Focusable::OnMove(Direction) {
    return false; //Default does not override movement
}

