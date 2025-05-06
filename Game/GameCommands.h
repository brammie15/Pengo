#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H
#include "Command.h"
#include "Components/PlayerComponent.h"

class PlayerMoveCommand: public fovy::BaseCommand {
public:
    explicit PlayerMoveCommand(fovy::PlayerComponent* component, fovy::MoveDirection dir);
    void Execute() override;
private:
    fovy::MoveDirection m_dir;
    fovy::PlayerComponent* m_pComponent;
};

#endif //GAMECOMMANDS_H
