#ifndef COMMAND_H
#define COMMAND_H

#include "./ObjectModel/GameObject.h"
#include <glm.hpp>

namespace fovy {
    class BaseCommand {
    public:
        virtual ~BaseCommand() = default;
        virtual void Execute() = 0;

        BaseCommand(const BaseCommand& other) = delete;
        BaseCommand(BaseCommand&& other) noexcept = delete;
        BaseCommand& operator=(const BaseCommand& other) = delete;
        BaseCommand& operator=(BaseCommand&& other) noexcept = delete;

    protected:
        BaseCommand() = default;
    };

    class GameObjectCommand: public BaseCommand {
    public:
        void Execute() override = 0;

    protected:
        explicit GameObjectCommand(GameObject* gameObject);
        [[nodiscard]] GameObject *GetGameObject() const { return m_GameObject; }

    private:
        GameObject* m_GameObject;
    };

    class MoveCommand: public GameObjectCommand {
    public:
        MoveCommand(GameObject* gameObject, float moveSpeed, glm::vec3 moveDirection);
        void Execute() override;

    private:
        float m_MoveSpeed;
        glm::vec3 m_MoveDirection;
    };

    class ConsoleLogCommand: public BaseCommand {
    public:
        explicit ConsoleLogCommand(const std::string& message);
        void Execute() override;
    private:
        std::string m_Message;
    };
}

#endif //COMMAND_H
