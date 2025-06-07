#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "Texture2D.h"
#include "ObjectModel/Component.h"
#include <SDL.h>

namespace fovy {
    struct Animation {
        std::vector<int> frames;
        float frameDuration;
        bool loop;

        Animation(std::vector<int> frames, float duration, bool looped = true)
            : frames(std::move(frames)), frameDuration(duration), loop(looped) {
        }
    };

    class SpriteRenderer: public Component {
    public:
        explicit SpriteRenderer(GameObject& pParent, const std::string& name = "SpriteRenderer");
        ~SpriteRenderer() override = default;

        void Update() override;
        void Render() override;

        // Texture management
        void SetTexture(const std::shared_ptr<Texture2D>& texture);
        void SetTexture(const std::string& filename);
        [[nodiscard]] const std::shared_ptr<Texture2D>& GetTexture() const { return m_Texture; }

        // Animation management
        void AddAnimation(const std::string& name, const std::vector<int>& frames, float frameTime = 0.1f, bool loop = true);
        void PlayAnimation(const std::string& name);
        void StopAnimation();
        [[nodiscard]] bool IsPlaying() const { return m_IsPlaying; }
        [[nodiscard]] const std::string& GetCurrentAnimation() const { return m_CurrentAnimation; }

        // Sprite properties
        void SetColor(const SDL_Color& color) { m_Color = color; }
        [[nodiscard]] const SDL_Color& GetColor() const { return m_Color; }
        void SetFlipHorizontal(bool flip) { m_FlipHorizontal = flip; }
        [[nodiscard]] bool GetFlipHorizontal() const { return m_FlipHorizontal; }
        void SetFlipVertical(bool flip) { m_FlipVertical = flip; }
        [[nodiscard]] bool GetFlipVertical() const { return m_FlipVertical; }

        // Tile properties
        void SetTileSize(int width, int height);
        void SetTileIndex(int index);
        [[nodiscard]] int GetTileIndex() const { return m_CurrentTileIndex; }

        void ImGuiInspector() override;

    private:
        struct Animation {
            std::vector<int> frames;
            float frameTime;
            float currentTime;
            int currentFrame;
            bool loop;
        };

        std::shared_ptr<Texture2D> m_Texture;
        SDL_Color m_Color{255, 255, 255, 255};
        bool m_FlipHorizontal{false};
        bool m_FlipVertical{false};

        // Tile properties
        int m_TileWidth{16};
        int m_TileHeight{16};
        int m_CurrentTileIndex{0};

        //TODO: make not hardcoded
        int m_DestTileWidth{46};
        int m_DestTileHeight{46};

        // Animation properties
        std::unordered_map<std::string, Animation> m_Animations;
        std::string m_CurrentAnimation;
        bool m_IsPlaying{false};

        void UpdateAnimation(float deltaTime);
        void CalculateSourceRect();
        SDL_Rect m_SourceRect{0, 0, 16, 16};
    };
} // namespace fovy

#endif // SPRITE_RENDERER_H
