#include <MyPacket.hpp>
#include <gameObject/Player.h>

/// Set the player spawn location
void Player::setPlayerSpawn(int x_pos, int y_pos) {
    spawn_xPos = x_pos;
    spawn_yPos = y_pos;
}

/// Move the player to the set spawn location
void Player::teleportPlayerSpawn() {
    sprite()->setX(static_cast<float>(spawn_xPos));
    sprite()->setY(static_cast<float>(spawn_yPos));
}

float Player::getPlayerSpeed() const {
    return player_speed;
}
[[maybe_unused]] void Player::setPlayerSpeed(float playerSpeed) {
    player_speed = playerSpeed;
}

/// Called from the main update() meaning this works as a Player::update()
/// function
void Player::playerUpdate() {
    playerMovement();
}

/// Control Player Movement. Boolean based
void Player::playerMovement() {
    if (downMovementKey && sprite()->getY() < 864) {
        sprite()->setY(sprite()->getY() + getPlayerSpeed());
    }
    if (upMovementKey && sprite()->getY() > 0) {
        sprite()->setY(sprite()->getY() - getPlayerSpeed());
    }
    if (rightMovementKey && sprite()->getX() < 1536) {
        sprite()->setX(sprite()->getX() + getPlayerSpeed());
    }
    if (leftMovementKey && sprite()->getX() > 0) {
        sprite()->setX(sprite()->getX() - getPlayerSpeed());
    }
}
