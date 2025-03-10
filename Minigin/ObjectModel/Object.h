#ifndef OBJECT_H
#define OBJECT_H
#include <string>

namespace fovy {
    class Object {
    public:
        Object(const Object& other) = delete;
        Object(Object&& other) noexcept = delete;
        Object& operator=(const Object& other) = delete;
        Object& operator=(Object&& other) noexcept = delete;

        [[nodiscard]] const std::string& GetName() const { return m_Name; }

        [[nodiscard]] bool IsBeingDestroyed() const { return m_BeingDestroyed; }

        void SetName(const std::string& newName){ m_Name = newName; }

        virtual ~Object();

        virtual void Destroy();

    protected:
        explicit Object(std::string name = "Object");

    private:
        bool m_BeingDestroyed{false};
        std::string m_Name{};
    };
} // dae

#endif //OBJECT_H
