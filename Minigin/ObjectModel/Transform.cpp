#include "Transform.h"

#include "GameObject.h"

fovy::Transform::Transform(GameObject* owner): m_Owner(owner) {

}

fovy::Transform::~Transform() {
    SetParent(nullptr);

    for (auto it = m_Children.begin(); it != m_Children.end();) {
        Transform* child = *it;
        it = m_Children.erase(it);
        child->SetParent(nullptr);
    }
}

const glm::vec3& fovy::Transform::GetWorldPosition() {
    if (m_PositionDirty) {
        UpdateWorldPosition();
    }
    return m_WorldPosition;
}

void fovy::Transform::SetWorldPosition(const glm::vec3& position) {
    if (m_Parent == nullptr) {
        SetLocalPosition(position);
    } else {
        SetLocalPosition(position - m_Parent->GetWorldPosition());
    }
}

void fovy::Transform::Move(const glm::vec3& move) {
    SetLocalPosition(m_LocalPosition + move);
}

void fovy::Transform::Move(double x, double y, double z) {
    this->Move(glm::vec3(x, y, z));
}

void fovy::Transform::SetLocalPosition(const glm::vec3& position) {
    m_LocalPosition = position;
    SetPositionDirty();
}

void fovy::Transform::RemoveChild(Transform* transform) {
    std::erase(m_Children, transform);
}

void fovy::Transform::AddChild(Transform* transform) {
    // if (transform == this or transform == nullptr) {
    //     return;
    // }
    //
    // if (transform->m_Parent) {
    //     transform->m_Parent->RemoveChild(transform);
    // }
    //
    // transform->SetParent(this);
    m_Children.push_back(transform);
    //
    // transform->SetPositionDirty();
}

void fovy::Transform::SetParent(Transform* parent, bool useWorldPosition) {
    if (parent == m_Parent or parent == this or IsChild(parent)) {
        return;
    }

    if (parent == nullptr) {
        SetLocalPosition(GetWorldPosition());
    } else {
        if (useWorldPosition) {
            SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
        }
        SetPositionDirty();
    }
    if (m_Parent) {
        m_Parent->RemoveChild(this);
    }
    m_Parent = parent;
    if (m_Parent) {
        m_Parent->AddChild(this);
    }
}


bool fovy::Transform::IsChild(fovy::Transform* child) const {
    return std::ranges::find(m_Children, child) != m_Children.end();
}

fovy::GameObject* fovy::Transform::GetOwner() const {
    return m_Owner;
}

void fovy::Transform::SetPositionDirty() {
    m_PositionDirty = true;
    for (auto child: m_Children) {
        child->SetPositionDirty();
    }
}

void fovy::Transform::UpdateWorldPosition() {
    if (m_Parent) {
        m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
    } else {
        m_WorldPosition = m_LocalPosition;
    }
    m_PositionDirty = false;
}
