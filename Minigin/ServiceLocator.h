
#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H
#pragma once
#include <memory>

#include "Audio/ISoundSystem.h"

namespace fovy {
    template<typename T>
    class ServiceLocator final {
    public:
        ServiceLocator() = delete;
        ~ServiceLocator() = delete;

        ServiceLocator(const ServiceLocator& other) = delete;
        ServiceLocator(ServiceLocator&& other) = delete;
        ServiceLocator& operator=(const ServiceLocator& other) = delete;
        ServiceLocator& operator=(ServiceLocator&& other) = delete;

        static T& GetService() { return *m_service; }
        static void RegisterService(std::unique_ptr<T>&& service) { m_service = std::move(service); }

    private:
        inline static std::unique_ptr<T> m_service { nullptr };
    };
}
#endif //SERVICELOCATOR_H
