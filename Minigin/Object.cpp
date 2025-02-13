#include "Object.h"

#include <utility>

dae::Object::~Object() {
}

void dae::Object::Destroy() {
}

dae::Object::Object(std::string name): m_Name(std::move(name)) {
}
