#include <SDL.h>
#include <SDL_image.h>
#include "Texture2D.h"
#include "./Managers/Renderer.h"
#include <stdexcept>

fovy::Texture2D::~Texture2D() {
    SDL_DestroyTexture(m_texture);
}

glm::ivec2 fovy::Texture2D::GetSize() const {
    SDL_Rect dst;
    SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
    return {dst.w, dst.h};
}

SDL_Texture* fovy::Texture2D::GetSDLTexture() const {
    return m_texture;
}

fovy::Texture2D::Texture2D(const std::string& fullPath) {
    m_texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
    if (m_texture == nullptr)
        throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
}

fovy::Texture2D::Texture2D(SDL_Texture* texture) : m_texture{texture} {
    assert(m_texture != nullptr);
}
