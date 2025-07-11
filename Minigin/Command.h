#ifndef COMMAND_H
#define COMMAND_H

#include <functional>

#include <glm.hpp>
#include "./ObjectModel/GameObject.h"

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

    class MoveCommand final: public GameObjectCommand {
    public:
        MoveCommand(GameObject* gameObject, float moveSpeed, glm::vec3 moveDirection);
        void Execute() override;

    private:
        float m_MoveSpeed;
        glm::vec3 m_MoveDirection;
    };

    class ConsoleLogCommand final: public BaseCommand {
    public:
        explicit ConsoleLogCommand(const std::string& message);
        void Execute() override;
    private:
        std::string m_Message;
    };

    class FunctionCommand final: public BaseCommand {
    public:
        explicit FunctionCommand(const std::function<void(void)>& function);
        void Execute() override;
    private:

        std::function<void(void)> m_function;
    };
}

#endif //COMMAND_H
