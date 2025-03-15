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

const glm::quat& fovy::Transform::GetWorldRotation() {
    if (m_RotationDirty) {
        UpdateWorldRotation();
    }
    return m_WorldRotation;
}

const glm::vec3& fovy::Transform::GetWorldScale() {
    if (m_ScaleDirty) {
        UpdateWorldScale();
    }
    return m_WorldScale;
}

const glm::mat4& fovy::Transform::GetWorldMatrix() {
    if (m_MatrixDirty) {
        UpdateWorldMatrix();
    }
    return m_WorldMatrix;
}

void fovy::Transform::SetWorldPosition(const glm::vec3& position) {
    if (m_Parent == nullptr) {
        SetLocalPosition(position);
    } else {
        SetLocalPosition(position - m_Parent->GetWorldPosition());
    }
}

void fovy::Transform::SetWorldPosition(float x, float y, float z) {
    SetWorldPosition(glm::vec3(x, y, z));
}

void fovy::Transform::SetWorldRotation(const glm::quat& rotation) {
    if(m_Parent == nullptr) {
        SetLocalRotation(rotation);
    } else {
        SetLocalRotation(glm::inverse(m_Parent->GetWorldRotation()) * rotation);
    }
}

void fovy::Transform::SetWorldRotation(const glm::vec3& rotation) {
    SetWorldRotation(glm::quat(glm::radians(rotation)));
}

void fovy::Transform::SetWorldRotation(float x, float y, float z) {
    SetWorldRotation(glm::vec3{x, y, z});
}

void fovy::Transform::SetWorldScale(double x, double y, double z) {
    SetWorldScale(glm::vec3{x, y, z});
}

void fovy::Transform::SetWorldScale(const glm::vec3& scale) {
    if (m_Parent == nullptr) {
        SetLocalScale(scale);
    } else {
        SetLocalScale(scale - m_Parent->GetWorldScale());
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

void fovy::Transform::SetLocalPosition(float x, float y, float z) {
    SetLocalPosition(glm::vec3{x, y, z});
}

void fovy::Transform::SetLocalRotation(float x, float y, float z) {
    SetLocalRotation(glm::vec3{x, y, z});
}

void fovy::Transform::SetLocalRotation(const glm::vec3& rotation) {
    SetLocalRotation(glm::quat(glm::radians(rotation)));
}

void fovy::Transform::SetLocalRotation(const glm::quat& rotation) {
    m_LocalRotation = rotation;
    SetRotationDirty();
}

void fovy::Transform::SetLocalScale(float x, float y, float z) {
    SetLocalScale({x, y, z});
}

void fovy::Transform::SetLocalScale(const glm::vec3& scale) {
    m_LocalScale = scale;
    SetScaleDirty();
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

fovy::GameObject *fovy::Transform::GetOwner() const {
    return m_Owner;
}

void fovy::Transform::SetPositionDirty() {
    m_PositionDirty = true;
    for (auto child: m_Children) {
        child->SetPositionDirty();
    }
}

void fovy::Transform::SetRotationDirty() {
    m_RotationDirty = true;
    m_MatrixDirty = true;

    for(Transform* childPtr : m_Children) {
        if(not childPtr->m_RotationDirty) {
            childPtr->SetRotationDirty();
        }
    }
}

void fovy::Transform::SetScaleDirty() {
    m_ScaleDirty = true;
    m_MatrixDirty = true;

    for(Transform* childPtr : m_Children) {
        if(not childPtr->m_ScaleDirty) {
            childPtr->SetScaleDirty();
        }
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

void fovy::Transform::UpdateWorldRotation() {
    if (m_Parent == nullptr) {
        m_WorldRotation = m_LocalRotation;
    } else {
        m_WorldRotation = m_LocalRotation * m_Parent->GetWorldRotation();
    }

    m_RotationDirty = false;
}

void fovy::Transform::UpdateWorldScale() {
    if(m_Parent == nullptr) {
        m_WorldScale = m_LocalScale;
    } else {
        m_WorldScale = m_LocalScale * m_Parent->GetWorldScale();
    }

    m_ScaleDirty = false;
}

void fovy::Transform::UpdateWorldMatrix() {
    const glm::mat4 trans = glm::translate(glm::mat4(1.0f), GetWorldPosition());
    const glm::mat4 rot = glm::mat4_cast(GetWorldRotation());
    const glm::mat4 scale = glm::scale(glm::mat4(1.0f), GetWorldScale());

    m_WorldMatrix = trans * rot * scale;
    m_MatrixDirty = false;
}


