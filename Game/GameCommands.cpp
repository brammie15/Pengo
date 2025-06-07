#include "GameCommands.h"

PlayerMoveCommand::PlayerMoveCommand(pengo::PengoComponent* component, pengo::MoveDirection dir): m_pComponent{component}, m_dir{dir} {
}

void PlayerMoveCommand::Execute() {
    m_pComponent->Move(m_dir);
}

PlayerPushCommand::PlayerPushCommand(pengo::PengoComponent* component): m_pComponent{component} {
}

void PlayerPushCommand::Execute() {
    m_pComponent->Push();
}
