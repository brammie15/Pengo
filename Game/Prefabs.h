#ifndef PREFABS_H
#define PREFABS_H
#include <vec3.hpp>

#include "Components/GridComponent.h"
#include "Components/Pengo/PengoComponent.h"
#include "Resources/Font.h"
#include "Scene/Scene.h"
#include "UI/Button.h"
#include "UI/Button.h"

namespace pengo {

    void SpawnPointsCollected(fovy::Scene& scene, glm::vec3 positon, int points);

    void AddEnemy(fovy::Scene* scene, GridComponent* grid, glm::vec2 gridPos, const std::string& name);

    void AddButtonToCanvas(
    const std::shared_ptr<fovy::GameObject>& button,
    const std::shared_ptr<fovy::GameObject>& canvas,
    const glm::vec3& position);

    fovy::GameObject *CreateLetterRoller(fovy::Scene* scene, fovy::GameObject* canvas, glm::vec3 position, std::shared_ptr<fovy::Font> font, const std::string& name);


    pengo::PengoComponent* CreatePlayer(fovy::Scene& scene, GridComponent* grid, const std::string& name = "Player1", const glm::ivec2& position = {});
}

#endif //PREFABS_H
