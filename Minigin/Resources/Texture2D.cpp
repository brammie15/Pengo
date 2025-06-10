#include "Texture2D.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include "../Managers/Renderer.h"

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

std::vector<std::vector<glm::vec4>> fovy::Texture2D::GetPixels() {
	if (m_texture == nullptr) {
		throw std::runtime_error("Texture is null");
	}

	auto* renderer = Renderer::GetInstance().GetSDLRenderer();
    
	SDL_Texture* texture = GetSDLTexture();

	int width, height;
    Uint32 format;
	SDL_QueryTexture(texture, &format, nullptr, &width, &height);

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format);
	if (surface == nullptr) {
		throw std::runtime_error(std::string("Failed to create surface: ") + SDL_GetError());
	}
	SDL_SetRenderTarget(renderer, texture);
	// SDL_LockSurface(surface);
	// SDL_RenderReadPixels(renderer, nullptr, format, surface->pixels, surface->pitch);
	SDL_SetRenderTarget(renderer, nullptr);

	SDL_LockSurface(surface);
	
	[[maybe_unused]] Uint32* pixels = static_cast<Uint32*>(surface->pixels);


	std::vector<std::vector<glm::vec4>> pixelData(height, std::vector<glm::vec4>(width));
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			Uint32 pixel = pixels[y * width + x];
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
			pixelData[y][x] = glm::vec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
		}
	}
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);


    return pixelData;
}

fovy::Texture2D::Texture2D(const std::string& fullPath) {
    m_texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
	m_path = fullPath.c_str();
    if (m_texture == nullptr) {
        throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
    }
}

fovy::Texture2D::Texture2D(SDL_Texture* texture) : m_texture{texture} {
    assert(m_texture != nullptr);
}
