#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H
#include "ObjectModel/Component.h"
#include "../Resources/Texture2D.h"
#include <memory>

namespace fovy {
    class TextureComponent final: public Component {
    public:

        explicit TextureComponent(GameObject &parent, std::shared_ptr<Texture2D> texture);

        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) noexcept = delete;
        TextureComponent& operator=(const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) noexcept = delete;

        void Update() override;
        void Render() override;

        void ImGuiInspector() override;

    private:
        std::shared_ptr<Texture2D> m_texture;
    };
} // dae

#endif //TEXTURECOMPONENT_H
