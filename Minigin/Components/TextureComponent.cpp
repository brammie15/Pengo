#include "TextureComponent.h"

#include <utility>

#include "Managers/Renderer.h"
#include "ObjectModel/Transform.h"

fovy::TextureComponent::TextureComponent(
    GameObject &parent,
    std::shared_ptr<Texture2D> texture): Component(parent, "TextureComponent"),
    m_texture(std::move(texture)) {
}

void fovy::TextureComponent::Update() {
}

void fovy::TextureComponent::Render() {
    const glm::vec3 renderPos = this->GetTransform().GetWorldPosition();
    const glm::vec3 renderScale = this->GetTransform().GetWorldScale();
//    const float renderRotation = this->GetTransform().GetWorldRotation().z;

    const auto textureSize =  m_texture->GetSize();

    Renderer::GetInstance().RenderTexture(
        *m_texture,
        renderPos.x,
        renderPos.y,
        static_cast<float>(textureSize.x) * renderScale.x,
        static_cast<float>(textureSize.y) * renderScale.y
    );

    // glm::vec2 center = {renderPos.x + (textureSize.x * renderScale.x) / 2.f, renderPos.y + (textureSize.y * renderScale.y) / 2.f};
    // Renderer::GetInstance().RenderLine(
    //     center.x, renderPos.y,
    //     center.x, renderPos.y + (textureSize.y * renderScale.y),
    //     SDL_Color(255, 0, 0, 255) // Red vertical line
    // );
    // Renderer::GetInstance().RenderLine(
    //     renderPos.x, center.y,
    //     renderPos.x + (textureSize.x * renderScale.x), center.y,
    //     SDL_Color(0, 255, 0, 255) // Green horizontal line
    // );
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
