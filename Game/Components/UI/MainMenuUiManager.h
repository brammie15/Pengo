
#ifndef MAINMENUUIMANAGER_H
#define MAINMENUUIMANAGER_H
#include "Event.h"
#include "ObjectModel/Component.h"
#include "Scene/SceneManager.h"


namespace pengo {
    class MainMenuUiManager: public fovy::EventListener, public fovy::Component {
    public:
        explicit MainMenuUiManager(fovy::GameObject& parent)
            : Component{parent, "MainMenuUiManager"} {

        }

        void OnStartGame() {
            fovy::SceneManager::GetInstance().SwitchScene(1);
        }

        void Update() override;
        void Destroy() override;

    private:
    };
}



#endif //MAINMENUUIMANAGER_H
