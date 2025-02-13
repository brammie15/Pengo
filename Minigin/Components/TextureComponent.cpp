#include "TextureComponent.h"

#include <utility>

#include "Renderer.h"
#include "Transform.h"

dae::TextureComponent::TextureComponent(
    GameObject* parent,
    std::shared_ptr<Texture2D> texture): Component(parent, "TextureComponent"),
    m_texture(std::move(texture)) {
}

void dae::TextureComponent::Update() {
}

void dae::TextureComponent::Render() {
    glm::vec3 renderPos = this->GetTransform().GetPosition();
    Renderer::GetInstance().RenderTexture(
        *m_texture,
        renderPos.x,
        renderPos.y
    );
}