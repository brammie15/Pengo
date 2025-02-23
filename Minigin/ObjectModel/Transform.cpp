#include "Transform.h"

#include "GameObject.h"

dae::Transform::Transform(GameObject* owner): m_Owner(owner) {

}

dae::Transform::~Transform() {
    SetParent(nullptr);

    for (auto it = m_Children.begin(); it != m_Children.end();) {
        Transform* child = *it;
        it = m_Children.erase(it);
        child->SetParent(nullptr);
    }
}

const glm::vec3& dae::Transform::GetWorldPosition() {
    if (m_PositionDirty) {
        UpdateWorldPosition();
    }
    return m_WorldPosition;
}

void dae::Transform::SetWorldPosition(const glm::vec3& position) {
    if (m_Parent == nullptr) {
        SetLocalPosition(position);
    } else {
        SetLocalPosition(position - m_Parent->GetWorldPosition());
    }
}

void dae::Transform::SetLocalPosition(const glm::vec3& position) {
    m_LocalPosition = position;
    SetPositionDirty();
}

void dae::Transform::RemoveChild(Transform* transform) {
    std::erase(m_Children, transform);
}

void dae::Transform::AddChild(Transform* transform) {
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

void dae::Transform::SetParent(Transform* parent, bool useWorldPosition) {
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


bool dae::Transform::IsChild(dae::Transform* child) const {
    return std::ranges::find(m_Children, child) != m_Children.end();
}

dae::GameObject* dae::Transform::GetOwner() const {
    return m_Owner;
}

void dae::Transform::SetPositionDirty() {
    m_PositionDirty = true;
    for (auto child: m_Children) {
        child->SetPositionDirty();
    }
}

void dae::Transform::UpdateWorldPosition() {
    if (m_Parent) {
        m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
    } else {
        m_WorldPosition = m_LocalPosition;
    }
    m_PositionDirty = false;
}
