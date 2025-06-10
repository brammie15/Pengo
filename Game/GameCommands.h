#ifndef GAMECOMMANDS_H
#define GAMECOMMANDS_H
#include "Command.h"
#include "Components/Pengo/PengoComponent.h"
#include "Components/SnoBee/SnoBeeComponent.h"

class PlayerMoveCommand final: public fovy::BaseCommand {
public:
    explicit PlayerMoveCommand(pengo::PengoComponent* component, MoveDirection dir);
    void Execute() override;
private:
    MoveDirection m_dir;
    pengo::PengoComponent* m_pComponent;
};

class PlayerPushCommand final: public fovy::BaseCommand {
public:
    explicit PlayerPushCommand(pengo::PengoComponent* component);
    void Execute() override;
private:
    pengo::PengoComponent* m_pComponent;
};

class SnoBeeMoveCommand final: public fovy::BaseCommand {
public:
    explicit SnoBeeMoveCommand(pengo::SnoBeeComponent* component, MoveDirection dir);
    void Execute() override;
private:
    MoveDirection m_dir;
    pengo::SnoBeeComponent* m_pComponent;
};



#endif //GAMECOMMANDS_H
