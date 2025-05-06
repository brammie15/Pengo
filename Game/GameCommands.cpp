#include "GameCommands.h"

PlayerMoveCommand::PlayerMoveCommand(fovy::PlayerComponent* component, fovy::MoveDirection dir): m_pComponent{component}, m_dir{dir} {
}

void PlayerMoveCommand::Execute() {
    m_pComponent->Move(m_dir);
}
