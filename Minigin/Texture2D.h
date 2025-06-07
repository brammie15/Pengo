#pragma once
#include <vec2.hpp>
#include <string>
#include <vec4.hpp>
#include <vector>

struct SDL_Texture;

namespace fovy {
    /**
     * Simple RAII wrapper for an SDL_Texture
     */
    class Texture2D final {
    public:
        [[nodiscard]] SDL_Texture *GetSDLTexture() const;

        explicit Texture2D(SDL_Texture* texture);
        explicit Texture2D(const std::string& fullPath);

        ~Texture2D();

        [[nodiscard]] glm::ivec2 GetSize() const;
        [[nodiscard]] std::vector<std::vector<glm::vec4>> GetPixels();

        [[nodiscard]] const std::string& GetPath() const { return m_path; }


        Texture2D(const Texture2D&) = delete;
        Texture2D(Texture2D&&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&&) = delete;

    private:
        SDL_Texture* m_texture;
        std::string m_path;
    };
}
