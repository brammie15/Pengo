#include "GameCommands.h"

PlayerMoveCommand::PlayerMoveCommand(pengo::PengoComponent* component, MoveDirection dir): m_pComponent{component}, m_dir{dir} {
}

void PlayerMoveCommand::Execute() {
    m_pComponent->Move(m_dir);
}

PlayerPushCommand::PlayerPushCommand(pengo::PengoComponent* component): m_pComponent{component} {
}

void PlayerPushCommand::Execute() {
    m_pComponent->Push();
}

SnoBeeMoveCommand::SnoBeeMoveCommand(pengo::SnoBeeComponent* component, MoveDirection dir): m_pComponent{component}, m_dir{dir} {
}

void SnoBeeMoveCommand::Execute() {
    m_pComponent->Move(m_dir);
}
