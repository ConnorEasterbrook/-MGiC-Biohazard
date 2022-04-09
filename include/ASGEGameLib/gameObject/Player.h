#ifndef NOTMARIOGAME_PLAYER_H
#define NOTMARIOGAME_PLAYER_H

#include "GameObject.h"
#include <Engine/Sprite.hpp>
#include <vector>

class Player : public GameObject
{
 public:
  Player()           = default;
  ~Player() override = default;

  void setPlayerSpawn(int x_pos, int y_pos);
  void teleportPlayerSpawn();
  [[nodiscard]] float getPlayerSpeed() const;
  [[maybe_unused]] void setPlayerSpeed(float playerSpeed);
  void playerUpdate();

  // PLAYER MOVEMENT
  bool leftMovementKey  = false;
  bool upMovementKey    = false;
  bool downMovementKey  = false;
  bool rightMovementKey = false;

  // PLAYER GAMEPLAY VARIABLES
  int score = 0;

 private:
  void playerMovement();

  Vector2 velocity   = { 0, 0 };
  int spawn_xPos     = 0;
  int spawn_yPos     = 0;
  float player_speed = 10;
};

#endif // NOTMARIOGAME_PLAYER_H
