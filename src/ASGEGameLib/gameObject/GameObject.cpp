#include <gameObject/GameObject.h>

void GameObject::goRender(ASGE::Renderer* renderer) const
{
  if (sprite_component)
  {
    if (visibility)
    {
      sprite_component->render(renderer);
    }
  }
}

bool GameObject::addSpriteComponent(
  const std::string& texture_file_path, std::unique_ptr<SpriteComponent> unique_sc,
  ASGE::Renderer* renderer, int z_order)
{
  sprite_component = std::move(unique_sc);
  sprite_component->newSprite(texture_file_path, renderer, z_order);

  return false;
}

bool GameObject::clickCollision(ASGE::SharedEventData data)
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

const std::unique_ptr<SpriteComponent>& GameObject::sprite() const
{
  return sprite_component;
}
