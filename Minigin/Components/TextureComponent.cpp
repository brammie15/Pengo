#include "TextureComponent.h"

#include <utility>

#include "Managers/Renderer.h"
#include "../ObjectModel/Transform.h"

fovy::TextureComponent::TextureComponent(
    GameObject &parent,
    std::shared_ptr<Texture2D> texture): Component(parent, "TextureComponent"),
    m_texture(std::move(texture)) {
}

void fovy::TextureComponent::Update() {
}

void fovy::TextureComponent::Render() {
    const glm::vec3 renderPos = this->GetTransform().GetWorldPosition();
    Renderer::GetInstance().RenderTexture(
        *m_texture,
        renderPos.x,
        renderPos.y
    );
}

void fovy::TextureComponent::ImGuiInspector() {
    SDL_Texture* texture = m_texture->GetSDLTexture();
    if (ImGui::TreeNode("TextureComponent")) {
        ImGui::Text("Texture");
        if (m_texture) {
            ImGui::Image((ImTextureID)(intptr_t)texture, ImVec2(100, 100));
        } else {
            ImGui::Text("No texture");
        }
        ImGui::TreePop();
    }
}
