#include "Command.h"
#include <iostream>
#include "Timer.h"

fovy::GameObjectCommand::GameObjectCommand(GameObject* gameObject): m_GameObject(gameObject) {
}

fovy::MoveCommand::MoveCommand(GameObject* gameObject, float moveSpeed, glm::vec3 moveDirection):
    GameObjectCommand(gameObject),
    m_MoveSpeed(moveSpeed),
    m_MoveDirection(moveDirection) {
}

void fovy::MoveCommand::Execute() {
    GetGameObject()->GetTransform().Move(glm::normalize(m_MoveDirection) * m_MoveSpeed * static_cast<float>(Time::GetInstance().DeltaTime()));
}

fovy::ConsoleLogCommand::ConsoleLogCommand(const std::string& message): m_Message(message) {
}

void fovy::ConsoleLogCommand::Execute() {
    std::cout << m_Message << std::endl;
}

fovy::FunctionCommand::FunctionCommand(const std::function<void()> function): m_function(function) {
}

void fovy::FunctionCommand::Execute() {
    m_function();
}
