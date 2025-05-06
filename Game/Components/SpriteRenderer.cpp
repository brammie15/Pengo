#include "SpriteRenderer.h"
#include "ObjectModel/Transform.h"
#include "Managers/Renderer.h"
#include "Timer.h"
#include "Managers/ResourceManager.h"

namespace fovy {

SpriteRenderer::SpriteRenderer(GameObject& pParent, const std::string& name)
    : Component(pParent, name) {}

void SpriteRenderer::Update() {
    if (m_IsPlaying) {
        UpdateAnimation(static_cast<float>(Time::GetInstance().DeltaTime()));
    }
}

void SpriteRenderer::Render() {
    if (!m_Texture || !this->isEnabled()) return;

    auto& transform = GetTransform();
    const auto& position = transform.GetWorldPosition();
    const auto& scale = transform.GetWorldScale();

    SDL_Rect destRect{
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        static_cast<int>(m_DestTileWidth * scale.x),
        static_cast<int>(m_DestTileHeight * scale.y)
    };

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (m_FlipHorizontal) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
    if (m_FlipVertical) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);

    SDL_SetTextureColorMod(m_Texture->GetSDLTexture(), m_Color.r, m_Color.g, m_Color.b);
    SDL_SetTextureAlphaMod(m_Texture->GetSDLTexture(), m_Color.a);
    SDL_RenderCopyEx(
        Renderer::GetInstance().GetSDLRenderer(),
        m_Texture->GetSDLTexture(),
        &m_SourceRect,
        &destRect,
        transform.GetWorldRotation().z * (180.f / static_cast<float>(M_PI)),
        nullptr,
        flip
    );
}

void SpriteRenderer::SetTexture(const std::shared_ptr<Texture2D>& texture) {
    m_Texture = texture;
    CalculateSourceRect();
}

void SpriteRenderer::SetTexture(const std::string& filename) {
    m_Texture = fovy::ResourceManager::GetInstance().LoadTexture(filename);
    CalculateSourceRect();
}

void SpriteRenderer::AddAnimation(const std::string& name, const std::vector<int>& frames, float frameTime, bool loop) {
    Animation anim;
    anim.frames = frames;
    anim.frameTime = frameTime;
    anim.currentTime = 0.0f;
    anim.currentFrame = 0;
    anim.loop = loop;
    m_Animations[name] = anim;
}

void SpriteRenderer::PlayAnimation(const std::string& name) {
    auto it = m_Animations.find(name);
    if (it != m_Animations.end()) {
        m_CurrentAnimation = name;
        m_IsPlaying = true;
        it->second.currentFrame = 0;
        it->second.currentTime = 0.0f;
        m_CurrentTileIndex = it->second.frames[0];
        CalculateSourceRect();
    }
}

void SpriteRenderer::StopAnimation() {
    m_IsPlaying = false;
}

void SpriteRenderer::SetTileSize(int width, int height) {
    m_TileWidth = width;
    m_TileHeight = height;
    CalculateSourceRect();
}

void SpriteRenderer::SetTileIndex(int index) {
    m_CurrentTileIndex = index;
    CalculateSourceRect();
}

void SpriteRenderer::UpdateAnimation(float deltaTime) {
    auto& anim = m_Animations[m_CurrentAnimation];
    anim.currentTime += deltaTime;

    if (anim.currentTime >= anim.frameTime) {
        anim.currentTime = 0.0f;
        anim.currentFrame++;

        if (anim.currentFrame >= anim.frames.size()) {
            if (anim.loop) {
                anim.currentFrame = 0;
            } else {
                m_IsPlaying = false;
                anim.currentFrame = static_cast<int>(anim.frames.size()) - 1;
            }
        }

        m_CurrentTileIndex = anim.frames[anim.currentFrame];
        CalculateSourceRect();
    }
}

void SpriteRenderer::CalculateSourceRect() {
    if (!m_Texture) return;

    int textureWidth, textureHeight;
    SDL_QueryTexture(m_Texture->GetSDLTexture(), nullptr, nullptr, &textureWidth, &textureHeight);

    int tilesPerRow = textureWidth / m_TileWidth;
    int row = m_CurrentTileIndex / tilesPerRow;
    int col = m_CurrentTileIndex % tilesPerRow;

    m_SourceRect.x = col * m_TileWidth;
    m_SourceRect.y = row * m_TileHeight;
    m_SourceRect.w = m_TileWidth;
    m_SourceRect.h = m_TileHeight;
}

} // namespace fovy