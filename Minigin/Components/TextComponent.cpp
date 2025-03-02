//
// Created by Bram on 10/02/2025.
//

#include "TextComponent.h"

#include <iostream>
#include <SDL_ttf.h>
#include <stdexcept>
#include <gtc/type_ptr.hpp>

#include "Managers/Renderer.h"
#include "Texture2D.h"
#include "../ObjectModel/Transform.h"

namespace dae {
    TextComponent::TextComponent(GameObject& parent, std::string text, std::shared_ptr<Font> font): Component(parent, "TEXT: " + text),
                                                              m_Text(text), m_Font(std::move(font)) {
    }

    void TextComponent::Render() {
        if (m_TextTexture != nullptr) {
            auto& position = this->GetTransform().GetWorldPosition();
            Renderer::GetInstance().RenderTexture(*m_TextTexture, position.x, position.y);
        }
    }

    void TextComponent::Update() {
        if (m_isDirty) {
            if (m_Text.length() == 0) {
                //No text was given can't really render that :/
                m_TextTexture = nullptr;
                m_isDirty = false;
                return;
            }

            const auto surface = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
            if (surface == nullptr) {
                std::cout << SDL_GetError() << std::endl;
                throw std::runtime_error(std::string("Render Text Failed") + SDL_GetError());
            }
            auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
            if (texture == nullptr) {
                throw std::runtime_error(std::string("Render Text Failed (Texture)") + SDL_GetError());
            }

            SDL_FreeSurface(surface);
            m_TextTexture = std::make_shared<Texture2D>(texture);
            m_isDirty = false;
        }
    }

    void TextComponent::SetText(const std::string& newText) {
        m_Text = newText;
        SetName("TEXT: " + newText);
        m_isDirty = true;
    }

    void TextComponent::ImGuiInspector() {
        if (ImGui::TreeNode("TextComponent")) {
            ImGui::Text("Current Text: ");
            ImGui::SameLine();
            ImGui::Text(m_Text.c_str());
            ImGui::Separator();
            ImGui::Text("Change Text");
            static char buffer[256]{};
            ImGui::InputText("##Text", buffer, sizeof(buffer));
            if (ImGui::Button("Set Text")) {
                SetText(std::string(buffer));
            }

            ImGui::SeparatorText("Color");

            glm::vec3 oldColor = glm::vec3{
                m_Color.r / 255.0f,
                m_Color.g / 255.0f,
                m_Color.b / 255.0f
            };

            if (ImGui::ColorEdit3("Color", glm::value_ptr(oldColor))) {
                m_Color = SDL_Color{
                    static_cast<Uint8>(oldColor.r * 255.0f),
                    static_cast<Uint8>(oldColor.g * 255.0f),
                    static_cast<Uint8>(oldColor.b * 255.0f),
                    static_cast<Uint8>(255)
                };
                m_isDirty = true;
            }


            ImGui::TreePop();
        }
    }
} // dae
