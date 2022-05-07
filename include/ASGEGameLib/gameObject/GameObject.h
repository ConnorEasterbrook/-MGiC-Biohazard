#ifndef NOTMARIOGAME_GAMEOBJECT_H
#define NOTMARIOGAME_GAMEOBJECT_H

#include "../utility/SpriteComponent.h"
#include "../utility/Vector2.h"
#include "iostream"
#include <Engine/InputEvents.hpp>
#include <Engine/Keys.hpp>
#include <string>

class GameObject {
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void goRender(ASGE::Renderer *renderer) const;
    virtual bool addSpriteComponent(
            const std::string &texture_file_path, std::unique_ptr<SpriteComponent> unique_sc,
            ASGE::Renderer *renderer, int z_order);
    virtual bool clickCollision(ASGE::SharedEventData data);

    [[nodiscard]] const std::unique_ptr<SpriteComponent> &sprite() const;

    bool visibility = false;

    std::unique_ptr<SpriteComponent> sprite_component = nullptr;
};

#endif// NOTMARIOGAME_GAMEOBJECT_H
