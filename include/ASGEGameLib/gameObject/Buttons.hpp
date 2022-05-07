//
// Created by robbi on 03/02/2022.
//

#ifndef ASGENETGAME_BUTTONS_HPP
#define ASGENETGAME_BUTTONS_HPP

#include "GameObject.h"

class Buttons : public GameObject {
public:
    Buttons() = default;
    ~Buttons() override = default;

    void setIsActive(bool change_activity);
    [[nodiscard]] bool getIsActive() const;
    void setSpriteSheetWidth(float x);
    [[nodiscard]] float getSpriteSheetWidth() const;
    void setSpriteSheetHeight(float y);
    [[nodiscard]] float getSpriteSheetHeight() const;
    void setSpriteScaler(float scale);
    [[nodiscard]] float getSpriteScaler() const;

    void goRender(ASGE::Renderer *renderer) const override;
    bool addSpriteComponent(
            const std::string &texture_file_path, std::unique_ptr<SpriteComponent> unique_sc,
            ASGE::Renderer *renderer, int z_order) override;
    bool clickCollision(ASGE::SharedEventData data) override;

private:
    bool is_active = false;
    float sprite_sheet_width = 57;
    float sprite_sheet_height = 18;
    float sprite_scaler = 4;
    float x_bound = 0;
    float y_bound = 18;
    float active_x_bound = 0;
    float active_y_bound = 0;
};

#endif// ASGENETGAME_BUTTONS_HPP
