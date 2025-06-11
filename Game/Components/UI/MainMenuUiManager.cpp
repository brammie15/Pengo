#include "MainMenuUiManager.h"

void pengo::MainMenuUiManager::Update() {
}

void pengo::MainMenuUiManager::Destroy() {
    Component::Destroy();
    std::puts("Destroyed");
}

