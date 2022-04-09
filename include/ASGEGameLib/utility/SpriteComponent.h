#ifndef NOTMARIOGAME_SPRITECOMPONENT_H
#define NOTMARIOGAME_SPRITECOMPONENT_H

#include "BoundingBox.h"
#include "vector"
#include <Engine/Renderer.hpp>
#include <Engine/Sprite.hpp>

class SpriteComponent
{
 public:
  SpriteComponent()  = default;
  ~SpriteComponent() = default;

  void render(ASGE::Renderer* renderer);
  void newSprite(const std::string& texture_file_name, ASGE::Renderer* renderer, int z_order);
  void setSprite(const std::string& texture_file_name);
  void setX(float x);
  void setY(float y);
  float getX();
  float getY();
  void setBounds(float x, float y);
  void setDimensions(float w, float h, float s);

  [[nodiscard]] BoundingBox getBoundingBox(float scale) const;
  [[nodiscard]] const std::unique_ptr<ASGE::Sprite>& getSprite() const;

 private:
  std::unique_ptr<ASGE::Sprite> sprite = nullptr;
  float width                          = 16;
  float height                         = 16;
  float src_x                          = 0;
  float src_y                          = 0;
};

#endif // NOTMARIOGAME_SPRITECOMPONENT_H
