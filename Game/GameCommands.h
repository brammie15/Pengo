#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H
#include "Command.h"
#include "Components/Pengo/PengoComponent.h"

class PlayerMoveCommand final: public fovy::BaseCommand {
public:
    explicit PlayerMoveCommand(pengo::PengoComponent* component, pengo::MoveDirection dir);
    void Execute() override;
private:
    pengo::MoveDirection m_dir;
    pengo::PengoComponent* m_pComponent;
};

class PlayerPushCommand final: public fovy::BaseCommand {
public:
    explicit PlayerPushCommand(pengo::PengoComponent* component);
    void Execute() override;
private:
    pengo::PengoComponent* m_pComponent;

};

#endif //GAMECOMMANDS_H
