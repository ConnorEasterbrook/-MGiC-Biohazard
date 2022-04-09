//
// Created by robbi on 03/02/2022.
//

#include "gameObject/Buttons.hpp"

void Buttons::setIsActive(bool change_activity)
{
  is_active = change_activity;
}

bool Buttons::getIsActive() const
{
  return is_active;
}

void Buttons::setSpriteSheetWidth(float x)
{
  sprite_sheet_width = x;
}

float Buttons::getSpriteSheetWidth() const
{
  return sprite_sheet_width;
}

void Buttons::setSpriteSheetHeight(float y)
{
  sprite_sheet_height = y;
}

float Buttons::getSpriteSheetHeight() const
{
  return sprite_sheet_height;
}

void Buttons::setSpriteScaler(float scale)
{
  sprite_scaler = scale;
}

float Buttons::getSpriteScaler() const
{
  return sprite_scaler;
}

void Buttons::goRender(ASGE::Renderer* renderer) const
{
  if (sprite_component)
  {
    if (is_active)
    {
      sprite()->setBounds(active_x_bound, active_y_bound);
    }
    else
    {
      sprite()->setBounds(x_bound, y_bound);
    }
    sprite_component->render(renderer);
  }
}

bool Buttons::addSpriteComponent(
  const std::string& texture_file_path, std::unique_ptr<SpriteComponent> unique_sc,
  ASGE::Renderer* renderer, int z_order)
{
  sprite_component = std::move(unique_sc);
  sprite_component->newSprite(texture_file_path, renderer, z_order);
  sprite()->setDimensions(sprite_sheet_width, sprite_sheet_height, sprite_scaler);
  sprite()->setBounds(x_bound, y_bound);

  return false;
}

bool Buttons::clickCollision(ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());

  if (click->action == ASGE::KEYS::KEY_PRESSED)
  {
    return static_cast<bool>(
      sprite()
        ->getBoundingBox(sprite()->getSprite()->scale())
        .collision(static_cast<float>(click->xpos), static_cast<float>(click->ypos), 0, 0));
  }
  return false;
}
