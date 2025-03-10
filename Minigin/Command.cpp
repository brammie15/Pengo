#include "Command.h"
#include <iostream>
#include "Timer.h"

dae::GameObjectCommand::GameObjectCommand(GameObject* gameObject): m_GameObject(gameObject) {
}

dae::MoveCommand::MoveCommand(GameObject* gameObject, float moveSpeed, glm::vec3 moveDirection):
    GameObjectCommand(gameObject),
    m_MoveSpeed(moveSpeed),
    m_MoveDirection(moveDirection) {
}

void dae::MoveCommand::Execute() {
    GetGameObject()->GetTransform().Move(glm::normalize(m_MoveDirection) * m_MoveSpeed * static_cast<float>(Time::GetInstance().DeltaTime()));
}

dae::ConsoleLogCommand::ConsoleLogCommand(const std::string& message): m_Message(message) {
}

void dae::ConsoleLogCommand::Execute() {
    std::cout << m_Message << std::endl;
}
