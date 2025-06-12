#ifndef DIRECTION_H
#define DIRECTION_H

#include <glm.hpp>

namespace fovy {
    enum class Direction {
        Up,
        Down,
        Left,
        Right,
        None
    };

    inline glm::vec2 DirectionToVector(Direction dir) {
        switch (dir) {
            case Direction::Up:    return {0.0f, -1.0f};
            case Direction::Down:  return {0.0f, 1.0f};
            case Direction::Left:  return {-1.0f, 0.0f};
            case Direction::Right: return {1.0f, 0.0f};
            default:               return {0.0f, 0.0f};
        }
    }

    inline Direction VectorToDirection(const glm::vec2& vec) {
        if (vec.y < 0) return Direction::Up;
        if (vec.y > 0) return Direction::Down;
        if (vec.x < 0) return Direction::Left;
        if (vec.x > 0) return Direction::Right;
        return Direction::None; // No movement
    }

}

#endif //DIRECTION_H
